#include "renderer/Camera.h"

const double Camera::RAY_MAX_DIST = 100.0;
Color Camera::BG_COLOR = WHITE;

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

Color Camera::ray_color(const Ray& ray, const MeshList& world, const LightsList& lights) const
{
    MeshHit rec;

    if (world.hit(ray, Range{0.0, RAY_MAX_DIST}, rec) && 0 < rec.t && rec.t < RAY_MAX_DIST) {
        return ray_trace(ShadeContext{
            &ray, &rec, &world, &lights
        });
        
        // return barycentric_shading(ShadeContext{
        //     nullptr,
        //     &rec,
        //     nullptr,
        //     nullptr
        // });
    }
    
    Vector3 unit_direction = normalized(ray.direction());
    auto a = 0.5*(unit_direction.y() + 1.0);
    return norm_vec_to_color(
        Vector3(1.0, 1.0, 1.0)*(1.0-a) +
        color_to_norm_vec(BG_COLOR)*a
    );
}
