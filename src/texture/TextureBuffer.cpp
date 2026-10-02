#include "texture/TextureBuffer.h"

TextureBuffer::TextureBuffer()
    : Grid2D()
{}

TextureBuffer::TextureBuffer(const int height, const int width)
    : Grid2D(height, width)
{}

TextureBuffer::TextureBuffer(const int height, const int width, const std::vector<Vector3> data)
    : Grid2D(height, width, data)
{}
