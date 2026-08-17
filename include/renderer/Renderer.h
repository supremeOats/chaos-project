#pragma once

#include "renderer/Scene.h"
#include "renderer/ImageBuffer.h"
#include <thread>

class Renderer
{
public:
    Renderer(const Scene scene, const int rayDepth = 4)
        : scene(scene), RAY_MAX_DEPTH(rayDepth), image(nullptr) {}
    
    void render(const char* imageFileName) const;
    void render_single_thread(std::ofstream& imageFile) const;
    
    float ratio() const { return (float)_width / _height; }
    unsigned width()    const  { return _width; }
    unsigned height()   const  { return _height; }

    void set_width(const unsigned w) { _width = w;}
    void set_height(const unsigned h) { _height = h; }

    void init_buffer();

private:
    void render_region(const PixelPos& start, const PixelPos& end) const;

    Vector3 ray_color(const Ray& ray, const int rayDepth) const;

    Vector3 shade_diffuse(const Ray& ray, const MeshHit& rec) const;
    Vector3 shade_reflective(const Ray& ray, const MeshHit& rec, const int rayDepth) const;
    Vector3 shade_refractive(const Ray& ray, const MeshHit& rec, const int rayDepth) const;
    Vector3 shade_constant(const Material& material) const;

    bool in_shadow(const MeshHit& rec, const Vector3 lightDir, const double lightDist) const;

    Vector3 gradient_bg(const Ray& ray) const;

private:
    const Scene scene;
    unsigned _height, _width;
    const int RAY_MAX_DEPTH;
    
    std::unique_ptr<ImageBuffer> image;

    static const double RAY_MAX_DIST;
};
