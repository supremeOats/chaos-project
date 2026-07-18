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
