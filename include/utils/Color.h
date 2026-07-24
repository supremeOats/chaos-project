#pragma once

#include <fstream>

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
