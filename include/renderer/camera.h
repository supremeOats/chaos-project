#pragma once

#include "geometry\Moveable.h"
#include "geometry\Hittable.h"
#include "utils\Color.h"
#include <fstream>

const int MAX_COLOR_COMPONENT = 255;

class Camera : public Moveable
{
public:
    Camera(
        const Point3& pos = {0, 0, 0},
        const Vector3& dir = {0, 0, -1},
        const float fov = 35,
        const float ratio = 16.0f/9,
        const unsigned width = 300
    );

    float    fov()      const  { return _fov; }
    float    ratio()    const  { return _ratio; }
    unsigned width()    const  { return _width; }
    unsigned height()   const  { return 1/_ratio * _width; }

    void render(const HittableList& scene, std::ofstream& imageFile) const;

private:
    Color ray_color(const Ray& ray, const HittableList& scene) const;

private:
    float _fov;
    float _ratio;
    unsigned _width;
};
