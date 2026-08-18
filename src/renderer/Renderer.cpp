#include "renderer/Renderer.h"

const double Renderer::RAY_MAX_DIST = 100.0;
const double SHADOW_BIAS = 0.01;
const double REFLECTION_BIAS = 0.01;
const double REFRACTION_BIAS = 0.01;
const double PI = 3.14159265358979323846;

void Renderer::set_dimentions(const unsigned w, const unsigned h)
{
    set_width(w);
    set_height(h);
    init_buffer();
}

void Renderer::render_single_thread(std::ofstream& imageFile) const
{
    imageFile << "P6 ";
    imageFile << _width << ' ' << _height << ' ';
    imageFile << MAX_COLOR_COMPONENT << '\n';

    for (size_t row = 0; row < _height; ++row) {
        for (size_t col = 0; col < _width; ++col) {
            float x = col + 0.5;
            x /= _width;
            x -= 0.5;
            x *= ratio();

            float y = row + 0.5;
            y /= _height;
            y -= 0.5;

            Ray ray = scene.camera->create_ray(x, y);

            imageFile << norm_vec_to_color(ray_color(ray, 0));
        }

        #ifdef DEBUG
        if(row % 100 == 0)
            std::cout << 100 * (float)(row) / (_height) << "%\n";
        #endif
    }
}

void Renderer::render(const char* imageFileName) const
{
    std::ofstream imageFile(imageFileName, std::ios::binary | std::ios::trunc);
    imageFile << "P6 ";
    imageFile << _width << ' ' << _height << ' ';
    imageFile << MAX_COLOR_COMPONENT << '\n';

    const int BUCKET_SIZE = 100;

    //schedule buckets
    struct Bucket { PixelPos start, end; };
    std::vector<Bucket> buckets;

    for (unsigned i = 0; i < _height; i += BUCKET_SIZE) {
        for (unsigned j = 0; j < _width; j += BUCKET_SIZE) {
            PixelPos start {i, j};
            PixelPos end {
                std::min(i + BUCKET_SIZE, _height),
                std::min(j + BUCKET_SIZE, _width)
            };
            buckets.push_back(Bucket{start, end});
        }
    }

    std::cout << "buckets assigned: " << buckets.size() << '\n';

    std::atomic<std::size_t> nextBucket{0};
    unsigned numThreads = std::thread::hardware_concurrency();
    if (numThreads == 0) numThreads = 4; // fallback if detection fails

    std::vector<std::thread> pool;
    for (unsigned t = 0; t < numThreads; ++t) {
        pool.emplace_back([&]() {
            std::size_t idx;
            while ((idx = nextBucket.fetch_add(1, std::memory_order_relaxed)) < buckets.size()) {
                const Bucket& b = buckets[idx];
                render_region(b.start, b.end);
            }
        });
    }

    for (auto& th : pool) {
        th.join();
    }

    
    image->write_to_file(imageFile);

    imageFile.close();
}

void Renderer::render_region(const PixelPos& start, const PixelPos& end) const
{
    if (start.x > end.x || start.y > end.y) {
        throw std::invalid_argument("end point of a region should be bottom-right to start point");
    }

    for (size_t row = start.x; row < end.x; ++row) {
        for (size_t col = start.y; col < end.y; ++col) {
            float x = col + 0.5;
            x /= _width;
            x -= 0.5;
            x *= ratio();

            float y = row + 0.5;
            y /= _height;
            y -= 0.5;

            Ray ray = scene.camera->create_ray(x, y);
            image->at(col, row) = norm_vec_to_color(ray_color(ray, 0));
        }
    }
}

Vector3 Renderer::ray_color(const Ray& ray, const int rayDepth) const
{
    if (rayDepth >= RAY_MAX_DEPTH) {
        return Vector3(0.0);
    }

    MeshHit rec;

    const MeshList& world = *scene.objects;
    const MaterialList& materials = *scene.materials;

    if (world.hit(ray, Range{0.0, RAY_MAX_DIST}, rec) && 0 < rec.t && rec.t < RAY_MAX_DIST) {        
        Vector3 finalColorVec(0, 0, 0);

        Material hitMaterial = materials[rec.materialIdx];
        
        switch (hitMaterial.type())
        {
        case DIFFUSE:
            finalColorVec = shade_diffuse(ray, rec);
            break;

        case REFLECTIVE:
            finalColorVec = shade_reflective(ray, rec, rayDepth);
            break;
        
        case REFRACTIVE:
            finalColorVec = shade_refractive(ray, rec, rayDepth);
            break;
        
        default: 
            break;
        }

        // if (hitMaterial.type() == REFLECTIVE) {
        //     finalColorVec = shade_reflective(ray, rec, rayDepth);
        // }
        // else if (hitMaterial.type() == REFRACTIVE) {
        //     finalColorVec = shade_refractive(ray, rec, rayDepth);
        // }
        // else if (hitMaterial.type() == CONSTANT) {
        //     finalColorVec = shade_constant(hitMaterial);
        // }
        // else {
        //     finalColorVec = shade_diffuse(ray, rec);
        // }
        
        return clamp(finalColorVec, {-1.0, 1.0});
    }
    
    return gradient_bg(ray);
}

Vector3 Renderer::shade_diffuse(const Ray& ray, const MeshHit& rec) const
{
    Vector3 finalColorVec(0, 0, 0);

    const LightsList& lights = *scene.lights;
    const MaterialList& materials = *scene.materials;

    Material hitMaterial = materials[rec.materialIdx];
    Vector3 hitNormal = hitMaterial.smooth() ? rec.pointNormal : rec.normal;

    for (const PointLight& lightSrc : lights) {
        Vector3 lp = lightSrc.pos() - rec.point;
        
        double lightVal = std::max(
            0.0,
            dot(hitNormal, normalized(lp))
        );

        if(lightVal == 0) {
            continue;
        }

        double lsRaduis = lp.length();

        if (!in_shadow(rec, lp, lsRaduis)) {
            double lsArea = 4 * PI * lsRaduis * lsRaduis;
            lightVal *= lightSrc.intensity() / lsArea;

            Vector3 colorVec = component_wise(
                color_to_norm_vec(lightSrc.color()),
                hitMaterial.albedo()
            );

            finalColorVec += colorVec * lightVal;
        }
    }
        
    return Vector3(
        std::min(1.0, finalColorVec.x()),
        std::min(1.0, finalColorVec.y()),
        std::min(1.0, finalColorVec.z())
    );
}

Vector3 Renderer::shade_reflective(const Ray& ray, const MeshHit& rec, const int rayDepth) const
{
    Material& hitMaterial = scene.materials->at(rec.materialIdx);
    Vector3 hitNormal = hitMaterial.smooth() ? rec.pointNormal : rec.normal;

    Vector3 materialAlbedo = scene.materials->at(rec.materialIdx).albedo();

    Ray reflected(
        rec.point + hitNormal * REFLECTION_BIAS,
        ray.direction() - hitNormal * 2 * dot(hitNormal, ray.direction())
    );
    
    return component_wise(materialAlbedo, ray_color(reflected, rayDepth+1));
}

Vector3 Renderer::shade_refractive(const Ray& ray, const MeshHit& rec, const int rayDepth) const
{
    Material& hitMaterial = scene.materials->at(rec.materialIdx);
    Vector3 hitNormal = hitMaterial.smooth() ? rec.pointNormal : rec.normal;
    
    double ior1 = ray.ior, ior2 = hitMaterial.ior();

    if (dot(ray.direction(), hitNormal) > 0.0) {
        std::swap(ior1, ior2);
        hitNormal *= -1;
    }

    double cosA = -1 * dot(ray.direction(), hitNormal);
    double sinA = std::sqrt(std::max(0.0, 1.0 - cosA * cosA));
    double sinB = std::sqrt(1 - cosA * cosA) * ior1 / ior2;
    double cosB = std::sqrt(1 - sinB * sinB);

    Ray reflected(
        rec.point + hitNormal * REFLECTION_BIAS,
        ray.direction() + hitNormal * cosA * 2,
        ior1
    );

    //Critical angle
    if (sinA > ior2/ior1) {
        return ray_color(reflected, rayDepth + 1);
    }

    Vector3 C = normalized(ray.direction() + hitNormal * cosA);
    Vector3 B = C * sinB;
    Vector3 A = hitNormal * -1 * cosB;

    Ray refracted(
        rec.point - hitNormal * REFRACTION_BIAS,
        A + B,
        ior2
    );

    double fresnelCoef = 0.5 * std::pow((1.0 - cosA), 5.0);

    return  fresnelCoef         * ray_color(reflected, rayDepth + 1)
            + (1 - fresnelCoef) * ray_color(refracted, rayDepth + 1);

    // check if entering or leaving refractive material
    //     if leabing then swap IORs and flip hit normal & proceed

    // check if angle between ray and normal is below critical
    //     if yes then build refraction ray & reflection ray & trace them aka return
    //     if not then build relection rat & trace it aka return
}

Vector3 Renderer::shade_constant(const Material& material) const
{
    return material.albedo();
}

bool Renderer::in_shadow(const MeshHit& rec, const Vector3 lightDir, const double lightDist) const
{
    //Cast shadow
    Ray shadowRay(
        rec.point,
        normalized(lightDir)
    );

    MeshHit shadowHit;
    
    if(scene.objects->hit(shadowRay, {SHADOW_BIAS, lightDist}, shadowHit)) {
        Material& material = scene.materials->at(shadowHit.materialIdx);
        return material.type() != REFRACTIVE;
    }
    
    return false;
}

Vector3 Renderer::gradient_bg(const Ray& ray) const
{
    Vector3 unit_direction = normalized(ray.direction());
    auto a = 0.5*(unit_direction.y() + 1.0);
    return Vector3(1.0, 1.0, 1.0)*(1.0-a) + color_to_norm_vec(scene.camera->BG_COLOR)*a;
}

void Renderer::init_buffer()
{
    image = std::make_unique<ImageBuffer> (_height, _width);
}

