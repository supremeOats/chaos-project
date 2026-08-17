#pragma once

#include "utils/Grid2d.h"
#include "utils/Color.h"

class ImageBuffer : public Grid2D<Color>
{
public:
    ImageBuffer(const int height, const int width);
        
    float ratio() const;

    void write_to_file(std::ofstream& imageFile);

private:
};
