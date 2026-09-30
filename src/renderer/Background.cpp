#include "renderer/Background.h"

FlatBG::FlatBG(const Vector3& color)
    : BG_COLOR(color)
{}

Vector3 FlatBG::get_color(const Ray&) const
{
    return BG_COLOR;
}

GradientBG::GradientBG(const Vector3& tColor, const Vector3& bColor)
    : TOP_COLOR(normalized(tColor)), BOTTOM_COLOR(normalized(bColor))
{}

Vector3 GradientBG::get_color(const Ray& ray) const
{
    Vector3 unit_direction = normalized(ray.direction());
    double a = 0.5*(unit_direction.y() + 1.0);

    return TOP_COLOR * a + BOTTOM_COLOR * (1.0-a);
}
