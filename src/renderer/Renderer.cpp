#include "renderer/Renderer.h"

const double Renderer::RAY_MAX_DIST = 100.0;
const double SHADOW_BIAS = 0.01;
const double PI = 3.14159265358979323846;

void Renderer::render(std::ofstream& imageFile) const
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

            imageFile << ray_color(ray, 0);
        }

        #ifdef DEBUG
        if(row % 100 == 0)
            std::cout << 100 * (float)(row) / (_height) << "%\n";
        #endif
    }
}

Color Renderer::ray_color(const Ray& ray, const int rayDepth) const
{
    MeshHit rec;

    const MeshList& world = *scene.objects;
    const MaterialList& materials = *scene.materials;

    if (world.hit(ray, Range{0.0, RAY_MAX_DIST}, rec) && 0 < rec.t && rec.t < RAY_MAX_DIST) {        
        Vector3 finalColorVec(0, 0, 0);

        Material hitMaterial = materials[rec.materialIdx];
        Vector3 hitNormal = hitMaterial.smooth() ? rec.pointNormal : rec.normal;
        
        if (hitMaterial.type() == REFLECTIVE && rayDepth < RAY_MAX_DEPTH) {
            finalColorVec += shade_reflective(ray, rec, rayDepth);
        }
        else if (hitMaterial.type() == REFLECTIVE && rayDepth < RAY_MAX_DEPTH) {
            finalColorVec += shade_refractive(ray, rec, rayDepth);
        }
        else {
            finalColorVec += shade_diffuse(ray, rec);
        }
        
        finalColorVec = Vector3(
            std::min(1.0, finalColorVec.x()),
            std::min(1.0, finalColorVec.y()),
            std::min(1.0, finalColorVec.z())
        );

        return norm_vec_to_color(finalColorVec);
    }
    
    return norm_vec_to_color(gradient_bg(ray));
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
    Vector3 materialAlbedo = scene.materials->at(rec.materialIdx).albedo();

    Ray reflectionRay(
        rec.point,
        ray.direction() - rec.normal * 2 * dot(rec.normal, ray.direction())
    );
    
    Color currColor = ray_color(reflectionRay, rayDepth+1);
    return component_wise(color_to_norm_vec(currColor), materialAlbedo);
}

Vector3 Renderer::shade_refractive(const Ray& ray, const MeshHit& rec, const int rayDepth) const
{
    double ior1, ior2;

    // check if entering or leaving refractive material
    //     if leabing then swap IORs and flip hit normal & proceed

    // check if angle between ray and normal is below critical
    //     if yes then build refraction ray & reflection ray & trace them aka return
    //     if not then build relection rat & trace it aka return

    return Vector3(0.0);
}

bool Renderer::in_shadow(const MeshHit& rec, const Vector3 lightDir, const double lightDist) const
{
    //Cast shadow
    Ray shadowRay(
        rec.point,
        normalized(lightDir)
    );
    MeshHit shadowHit;

    return scene.objects->hit(shadowRay, {SHADOW_BIAS, lightDist}, shadowHit);
}

Vector3 Renderer::gradient_bg(const Ray& ray) const
{
    Vector3 unit_direction = normalized(ray.direction());
    auto a = 0.5*(unit_direction.y() + 1.0);
    return Vector3(1.0, 1.0, 1.0)*(1.0-a) + color_to_norm_vec(scene.camera->BG_COLOR)*a;
}
