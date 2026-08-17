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
    Grid2D(const size_t height, const size_t width);

    const T& at(const int x, const int y) const;
    T& at(const int x, const int y);

    unsigned width() const { return _width; }
    unsigned height() const { return _height; }
    size_t size() const { return data.size(); }

protected:
    const size_t _height, _width;
    std::vector<T> data;
};

template <typename T>
Grid2D<T>::Grid2D(const size_t height, const size_t width)
    : _height(height), _width(width), data(_height * _width)
{}

template <typename T>
const T& Grid2D<T>::at(const int x, const int y) const
{
    return data.at(x * _height, y);
}

template <typename T>
T& Grid2D<T>::at(const int x, const int y)
{
    return data.at(y * _width + x);
}

