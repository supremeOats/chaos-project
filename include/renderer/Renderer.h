#pragma once

#include "renderer/Scene.h"

class Renderer
{
public:
    Renderer(const Scene scene)
        : scene(scene) {}
    
    void render(std::ofstream& imageFile) const;
    
    float ratio() const { return (float)_width / _height; }
    unsigned width()    const  { return _width; }
    unsigned height()   const  { return _height; }

    void set_width(const unsigned w) { _width = w;}
    void set_height(const unsigned h) { _height = h; }

private:
    const Scene scene;

    float _height;
    unsigned _width;
};
