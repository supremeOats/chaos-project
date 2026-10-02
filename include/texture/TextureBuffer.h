#pragma once

#include "utils/Grid2d.h"
#include "geometry/Vector3.h"

class TextureBuffer : public Grid2D<Vector3>
{
public:
    TextureBuffer();
    TextureBuffer(const int height, const int width);
    TextureBuffer(const int height, const int width, const std::vector<Vector3> data);
};
