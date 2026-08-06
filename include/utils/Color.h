#pragma once

#include <fstream>
#include "geometry/Vector3.h"

struct Color
{
    unsigned char r;
    unsigned char g;
    unsigned char b;

    friend std::ofstream& operator<< (std::ofstream& f, const Color& c)
    {
        f.write((const char*) &c.r, sizeof(char));
        f.write((const char*) &c.g, sizeof(char));
        f.write((const char*) &c.b, sizeof(char));

        return f;
    }
};

static const Color BLACK {0, 0, 0};
static const Color WHITE {255, 255, 255};
static const Color RED   {255, 0, 0};
static const Color GREEN {0, 255, 0};
static const Color BLUE  {0, 0, 255};
static const Color CYAN  {0, 255, 255};
static const Color YELLOW  {255, 255, 0};

Color norm_vec_to_color(const Vector3& vec);
Vector3 color_to_norm_vec(const Color& col);

/*
Color operator+ (const Color& lhs, const Color& rhs)
{
    return Color(
        lhs.r && rhs.r,
        lhs.g && rhs.g,
        lhs.b && rhs.b
    );
}
*/
