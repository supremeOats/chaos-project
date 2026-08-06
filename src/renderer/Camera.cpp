#include "renderer/Camera.h"

const double Camera::RAY_MAX_DIST = 100.0;
Color Camera::BG_COLOR = WHITE;

# define PI 3.14159265358979323846

Camera::Camera(const Point3& pos, const Vector3& dir, const float fov)
    : Moveable(pos, dir), _fov(fov)
{}

Ray Camera::make_ray(const float x, const float y) const
{
    return Ray(
        pos(),
        normalized(this->rotation_matrix() * Vector3(x, -y, -0.5f))
    );
}

Vector3 Camera::shade(const PointLight& light, const Hit& record) const
{
    Vector3 lp = light.pos() - record.point;
    
    double lightVal = std::max(
        0.0,
        dot(record.normal, normalized(lp))
    );

    double lsRaduis = lp.length();
    double lsArea = 4 * PI * lsRaduis * lsRaduis;

    lightVal *= light.intensity() / lsArea;
    
    Vector3 colorVec = color_to_norm_vec(light.color());

    return  Vector3(
        lightVal * colorVec.x(),
        lightVal * colorVec.y(),
        lightVal * colorVec.z()
    );
}

Color Camera::ray_color(const Ray& ray, const HittableList& world, const LightsList& lights) const
{
    Hit rec;

    if (world.hit(ray, RAY_MAX_DIST, rec) && 0 < rec.t && rec.t < RAY_MAX_DIST) {
        Vector3 finalColorVec(0, 0, 0);

        for (const PointLight& lightSrc : lights) {
            finalColorVec += shade(lightSrc, rec);
        }

        finalColorVec = Vector3(
            std::min(1.0, finalColorVec.x()),
            std::min(1.0, finalColorVec.y()),
            std::min(1.0, finalColorVec.z())
        );

        return norm_vec_to_color(finalColorVec);
    }
    
    return BG_COLOR;
}
