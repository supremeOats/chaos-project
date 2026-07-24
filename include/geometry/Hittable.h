#pragma once

#include <vector>
#include <memory>
#include "Ray.h"

struct Hit
{
    double t;
    Vector3 normal;
    Point3 point;
};

class Hittable
{
public:
    virtual ~Hittable() = default;

    virtual bool hit(const Ray& ray, Hit& hitData) const = 0;
};

class HittableList : private Hittable
{
public:
    bool hit(const Ray& ray, Hit& hitData) const override;

    void add(std::shared_ptr<Hittable> obj);

private:
    std::vector<std::shared_ptr<Hittable>> objects;
};
