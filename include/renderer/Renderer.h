#pragma once

#include "renderer/Scene.h"

class Renderer
{
public:
    Renderer(const Scene scene, const int rayDepth = 3)
        : scene(scene), RAY_MAX_DEPTH(rayDepth) {}
    
    void render(std::ofstream& imageFile) const;
    
    float ratio() const { return (float)_width / _height; }
    unsigned width()    const  { return _width; }
    unsigned height()   const  { return _height; }

    void set_width(const unsigned w) { _width = w;}
    void set_height(const unsigned h) { _height = h; }

private:
    Color Renderer::ray_color(const Ray& ray, const int rayDepth) const;

    Vector3 shade_diffuse(const Ray& ray, const MeshHit& rec) const;
    Vector3 shade_reflective(const Ray& ray, const MeshHit& rec, const int rayDepth) const;
    Vector3 shade_refractive(const Ray& ray, const MeshHit& rec, const int rayDepth) const;

    bool in_shadow(const MeshHit& rec, const Vector3 lightDir, const double lightDist) const;

    Vector3 gradient_bg(const Ray& ray) const;

private:
    const Scene scene;

    unsigned _height;
    unsigned _width;

    const int RAY_MAX_DEPTH;

    static const double Renderer::RAY_MAX_DIST;
};
