#pragma once

#include "geometry/Moveable.h"
#include "geometry/Mesh.h"
#include "light/Light.h"
#include <fstream>

#include "shading/ShadingFunctions.h"

const int MAX_COLOR_COMPONENT = 255;

class Camera : public Moveable
{
public:
    Camera(
        const Point3& pos = {0, 0, 0},
        const Vector3& dir = {0, 0, -1}
    );

    Ray make_ray(const float x, const float y) const;
    Color Camera::ray_color(const Ray& ray, const MeshList& world, const LightsList& lights, const MaterialList& materials, const int rayDepth) const;
    
    static Color BG_COLOR;

private:
    static const double RAY_MAX_DIST;
    static const int RAY_MAX_DEPTH;
};
