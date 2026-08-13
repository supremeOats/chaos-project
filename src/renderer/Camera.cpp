#include "renderer/Camera.h"

const double Camera::RAY_MAX_DIST = 100.0;
const int Camera::RAY_MAX_DEPTH = 3;
Color Camera::BG_COLOR = WHITE;

# define PI 3.14159265358979323846
const double SHADOW_BIAS = 0.01;

Camera::Camera(const Point3& pos, const Vector3& dir)
    : Moveable(pos, dir)
{}

Ray Camera::make_ray(const float x, const float y) const
{
    return Ray(
        pos(),
        normalized(this->rotation_matrix() * Vector3(x, -y, -0.65))
    );
}

Color Camera::ray_color(const Ray& ray, const MeshList& world, const LightsList& lights, const MaterialList& materials, const int rayDepth) const
{
    MeshHit rec;

    if (world.hit(ray, Range{0.0, RAY_MAX_DIST}, rec) && 0 < rec.t && rec.t < RAY_MAX_DIST) {        
        Vector3 finalColorVec(0, 0, 0);

        Material hitMaterial = materials[rec.materialIdx];
        Vector3 hitNormal = hitMaterial.smooth() ? rec.pointNormal : rec.normal;
        
        if (hitMaterial.type() == REFLECTIVE && rayDepth < RAY_MAX_DEPTH) {
            Ray reflectionRay(
                rec.point,
                ray.direction() - rec.normal * 2 * dot(rec.normal, ray.direction())
            );
            
            Color currColor = ray_color(reflectionRay, world, lights, materials, rayDepth+1);
            finalColorVec = component_wise(color_to_norm_vec(currColor), hitMaterial.albedo());
        }
        else {
            for (const PointLight& lightSrc : lights) {
            Vector3 lp = lightSrc.pos() - rec.point;
            
            double lightVal = std::max(
                0.0,
                dot(hitNormal, normalized(lp))
            );

            if(lightVal == 0) {
                finalColorVec += Vector3(0, 0, 0);
            }

            double lsRaduis = lp.length();

            //Cast shadow
            Ray shadowRay(
                rec.point,
                normalized(lp)
            );
            MeshHit shadowHit;

            if (world.hit(shadowRay, {SHADOW_BIAS, lsRaduis}, shadowHit)) {
                finalColorVec += Vector3(0, 0, 0);
            }

            //Light contribution
            double lsArea = 4 * PI * lsRaduis * lsRaduis;
            lightVal *= lightSrc.intensity() / lsArea;

            Vector3 colorVec = component_wise(
                color_to_norm_vec(lightSrc.color()),
                hitMaterial.albedo()
            );

            finalColorVec += colorVec * lightVal;
        }
        }
        
        finalColorVec = Vector3(
            std::min(1.0, finalColorVec.x()),
            std::min(1.0, finalColorVec.y()),
            std::min(1.0, finalColorVec.z())
        );

        return norm_vec_to_color(finalColorVec);
    }
    
    //BG color
    Vector3 unit_direction = normalized(ray.direction());
    auto a = 0.5*(unit_direction.y() + 1.0);
    return norm_vec_to_color(
        Vector3(1.0, 1.0, 1.0)*(1.0-a) +
        color_to_norm_vec(BG_COLOR)*a
    );
}
