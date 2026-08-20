#pragma once

#include "geometry/Vector3.h"
#include "utils/Color.h"
#include "geometry/Moveable.h"

//class BaseLight

class PointLight : public Moveable
{
public:
    PointLight(const Point3& position, const Color& color = WHITE, const float intensity = 1.0f)
        : Moveable(position, 0.0), _col(color), _intensity(intensity) {}

    const Point3& pos() const { return _pos; }
    const Color& color() const { return _col; }
    float intensity() const { return _intensity; }

private:
    Color _col;
    float _intensity;
};

using LightsList = std::vector<PointLight>;
