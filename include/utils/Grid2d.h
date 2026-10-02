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
    Grid2D(const size_t height, const size_t width, const std::vector<T> data);

    const T& at(const size_t x, const size_t y) const;
    T& at(const size_t x, const size_t y);

    T& at(const PixelPos& pos);
    const T& at(const PixelPos& pos) const;

    size_t width() const { return _width; }
    size_t height() const { return _height; }
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
Grid2D<T>::Grid2D(const size_t height, const size_t width, const std::vector<T> data)
    : Grid2D(height, width)
{
    std::copy(data.begin(), data.end(), this->data.begin());
}

template <typename T>
const T& Grid2D<T>::at(const size_t x, const size_t y) const
{
    return data.at(y * _width + x);
}

template <typename T>
T& Grid2D<T>::at(const size_t x, const size_t y)
{
    return data.at(y * _width + x);
}

template <typename T>
T& Grid2D<T>::at(const PixelPos& pos)
{
    return Grid2D::at(pos.x, pos.y);
}

template <typename T>
const T& Grid2D<T>::at(const PixelPos& pos) const
{
    return Grid2D::at(pos.x, pos.y);
}
