#include "renderer/Camera.h"

const int Camera::RAY_MAX_DEPTH = 3;
Color Camera::BG_COLOR = WHITE;

# define PI 3.14159265358979323846
const double SHADOW_BIAS = 0.01;

Camera::Camera(const Point3& pos, const Vector3& dir)
    : Moveable(pos, dir)
{
    update_viewport();
}

Ray Camera::create_ray(const float x, const float y, const float w, const float h) const
{
    float px = x * viewportScale;
    float py = -y * viewportScale;

    return Ray(
        _pos,
        normalized(this->rotation_matrix() * Vector3(px, py, -1.0))
    );

    // return Ray(
    //     pos(),
    //     normalized(this->rotation_matrix() * Vector3(x, -y, -0.65))
    // );
}

void Camera::set_fov(const double fov)
{
    _fovDeg = clamp(fov, 1.0, 179.0);
    _fovDeg = fov;

    update_viewport();
}

void Camera::update_viewport()
{
    viewportScale = std::tan((_fovDeg * PI/180) / 2.0f);
}

