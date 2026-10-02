#pragma once

#include "utils/Grid2d.h"
#include "geometry/Vector3.h"
#include "texture/TextureBuffer.h"

class BaseTexture
{
public:
    BaseTexture(const std::string& name) : _name(name) {}
    virtual ~BaseTexture() = default;

    virtual const Vector3& at(double u, double v) const = 0;

    const std::string& name() const { return _name; }

private:
    std::string _name;
};

class ProceduralTexture : public BaseTexture
{
public:
    ProceduralTexture(const std::string& name) : BaseTexture(name) {}
    virtual ~ProceduralTexture() = default;

    virtual const Vector3& at(double u, double v) const = 0;
};

class ColorTexture : public ProceduralTexture
{
public:
    ColorTexture(const std::string& name, const Vector3& color);

    virtual const Vector3& at(double, double) const override;

private:
    Vector3 color;
};

class Bitmap : public BaseTexture
{
public:
    Bitmap(const std::string& name, int height, int width, const std::vector<Vector3>& pixels);

    virtual const Vector3& at(double u, double v) const = 0;

private:
    TextureBuffer pixelData;
};

using TextureList = std::vector<std::shared_ptr<BaseTexture>>;

int find_texture_idx(const TextureList& list, const std::string& name);
