#include "renderer/ImageBuffer.h"

ImageBuffer::ImageBuffer(const int height, const int width) : Grid2D(height, width) {}
        
float ImageBuffer::ratio() const
{
    return (float)_width / _height;
}

void ImageBuffer::write_to_file(std::ofstream& imageFile)
{
    imageFile.write(reinterpret_cast<const char*>(&data[0]), data.size() * sizeof(Color));
}
