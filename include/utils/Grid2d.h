#pragma once

#include <vector>
#include <iostream>

struct PixelPos
{
    size_t x, y;
};

template <typename T>
class Grid2D
{
public:
    Grid2D() : _height(0), _width(0), data() {}
    Grid2D(const size_t height, const size_t width);

    const T& at(const size_t x, const size_t y) const;
    T& at(const size_t x, const size_t y);

    unsigned width() const { return _width; }
    unsigned height() const { return _height; }
    size_t size() const { return data.size(); }

    // void set_dimentions(const size_t height, const size_t width) const;

protected:
    const size_t _height, _width;
    std::vector<T> data;
};

template <typename T>
Grid2D<T>::Grid2D(const size_t height, const size_t width)
    : _height(height), _width(width), data(_height * _width)
{}

template <typename T>
const T& Grid2D<T>::at(const size_t x, const size_t y) const
{
    return data.at(x * _height, y);
}

template <typename T>
T& Grid2D<T>::at(const size_t x, const size_t y)
{
    return data.at(y * _width + x);
}

// template <typename T>
// void Grid2D<T>::set_dimentions(const size_t height, const size_t width) const
// {
//     _height = height;
//     _width = width;

//     std::vector<T> newData(_height * _width);
//     data.clear();
//     data = newData;
    
//     //TODO: to copy, or not to copy
//     //should data content be copied to newData? what if newData is smaller?
// }

