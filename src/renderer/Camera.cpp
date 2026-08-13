#include "renderer/Camera.h"

const int Camera::RAY_MAX_DEPTH = 3;
Color Camera::BG_COLOR = WHITE;

# define PI 3.14159265358979323846
const double SHADOW_BIAS = 0.01;

Camera::Camera(const Point3& pos, const Vector3& dir)
    : Moveable(pos, dir)
{}

Ray Camera::create_ray(const float x, const float y) const
{
    return Ray(
        pos(),
        normalized(this->rotation_matrix() * Vector3(x, -y, -0.65))
    );
}
