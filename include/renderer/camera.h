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
        const Vector3& dir = {0, 0, -1},
        const float fov = 35
    );

    float fov() const { return _fov; }
    
    Ray make_ray(const float x, const float y) const;
    Color ray_color(const Ray& ray, const MeshList& scene, const LightsList& lights) const;
    
    static Color BG_COLOR;

private:
    Vector3 shade(const PointLight& light, const MeshHit& record, const MeshList& world) const;

private:
    float _fov;

    static const double RAY_MAX_DIST;
};
