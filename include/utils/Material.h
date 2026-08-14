#pragma once

#include "geometry/Vector3.h"
#include <vector>

enum MaterialType {
    DIFFUSE, REFLECTIVE, REFRACTIVE, CONSTANT
};

class Material
{
public:
    Material(const MaterialType type, const Vector3& albedo, const bool smooth, const double ior)
        : _type(type), _albedo(albedo), _smooth(smooth), _ior(ior) {}

    MaterialType type() const { return _type; }
    
    const Vector3& albedo() const { return _albedo; }
    
    bool smooth() const { return _smooth; }

    double ior() const { return _ior; }

private:
    MaterialType _type;
    Vector3 _albedo;
    bool _smooth;
    double _ior;
};

using MaterialList = std::vector<Material>;
