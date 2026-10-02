#pragma once

#include "texture/Texture.h"

enum MaterialType {
    DIFFUSE, REFLECTIVE, REFRACTIVE, CONSTANT
};

class Material
{
public:
    Material(const MaterialType type, int textureIdx, const bool smooth, const double ior)
        : _type(type), textureIdx(textureIdx), _smooth(smooth), _ior(ior) {}

    MaterialType type() const { return _type; }
    
    int texture_index() const { return textureIdx; }
    // const Vector3& albedo(double u, double v, ) const { return texture->at(u, v); }
    
    bool smooth() const { return _smooth; }

    double ior() const { return _ior; }

private:
    MaterialType _type;
    // Vector3 _albedo;
    int textureIdx;

    bool _smooth;
    double _ior;
};

using MaterialList = std::vector<Material>;
