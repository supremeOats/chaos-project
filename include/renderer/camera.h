#pragma once

#include "geometry/Moveable.h"
#include "geometry/Ray.h"
#include "utils/Color.h"
#include <algorithm>

const int MAX_COLOR_COMPONENT = 255;

class Camera : public Moveable
{
public:
    Camera(
        const Point3& pos = {0, 0, 0},
        const Vector3& dir = {0, 0, -1}
    );

    // Ray create_ray(const float x, const float y) const;
    Ray create_ray(const float x, const float y, const float w, const float h) const;

    void set_fov(const double fov);
    
    static Color BG_COLOR;
    
private:
    void update_viewport();

private:
    static const double RAY_MAX_DIST;
    static const int RAY_MAX_DEPTH;

    double _fovDeg = 90;
    double viewportScale;
};
