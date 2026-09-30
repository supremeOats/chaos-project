#pragma once

#include "geometry/Ray.h"

enum BG_TYPE {
    FLAT, GRADIENT, SKYBOX
};

class BaseBG
{
public:
    virtual ~BaseBG() = default;

    virtual Vector3 get_color(const Ray& ray) const = 0;
};

class FlatBG : public BaseBG
{
public:
    FlatBG(const Vector3& color);

    virtual Vector3 get_color(const Ray& ray) const override;

private:
    const Vector3 BG_COLOR = (1.0);
};

class GradientBG : public BaseBG
{
public:
    GradientBG(const Vector3& tColor, const Vector3& bColor);

    virtual Vector3 get_color(const Ray& ray) const override;

private:
    const Vector3 TOP_COLOR = (0.0, 0.2, 0.8);
    const Vector3 BOTTOM_COLOR = (1.0);
};
