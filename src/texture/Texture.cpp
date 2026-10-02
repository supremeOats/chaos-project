#include "texture/Texture.h"

const Vector3& ColorTexture::at(double, double) const
{
    return color;
}

const Vector3& Bitmap::at(double u, double v) const
{
    return pixelData.at(
        (size_t)(v * pixelData.height()),
        (size_t)(u * pixelData.width())
    );
}

ColorTexture::ColorTexture(const std::string& name, const Vector3& color)
    : ProceduralTexture(name), color(color)
{}

Bitmap::Bitmap(const std::string& name, int height, int width, const std::vector<Vector3>& pixels)
    : BaseTexture(name), pixelData(height, width, pixels)
{}

int find_texture_idx(const TextureList& list, const std::string& name)
{
    for (int index = 0; index < list.size(); ++index) {
        if (std::strcmp(list[index]->name().c_str(), name.c_str()) == 0) {
            return index;
        }
    }

    return -1;
}
