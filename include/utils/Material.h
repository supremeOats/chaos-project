#pragma once

#include "geometry/Vector3.h"
#include <vector>

enum MaterialType {
    DIFFUSE, REFLECTIVE, REFRACTIVE
};

class Material
{
public:
    Material(const MaterialType type, const Vector3& albedo, const bool smooth)
        : _type(type), _albedo(albedo), _smooth(smooth) {}

    MaterialType type() const { return _type; }
    const Vector3& albedo() const { return _albedo; }
    bool smooth() const { return _smooth; }

private:
    MaterialType _type;
    Vector3 _albedo;
    bool _smooth;
};

using MaterialList = std::vector<Material>;
