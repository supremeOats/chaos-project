#pragma once

#include "Hittable.h"
#include "geometry\Moveable.h"
#include "Triangle.h"

class Mesh : public Hittable, public Moveable
{
public:
    ~Mesh() = default;

    virtual bool hit(const Ray& ray, Hit& hitData) const;

private:
    std::vector<Triangle> geometry;
    //todo: replace vector with tree Bounding Box structure
};
