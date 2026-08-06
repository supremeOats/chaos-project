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

struct Range
{
    double minDist;
    double maxDist;
};

class Hittable
{
public:
    virtual ~Hittable() = default;

    virtual bool hit(const Ray& ray, const Range& rayRange, Hit& hitData) const = 0;
};

// class Mesh : public Hittable
// {};

class HittableList : public Hittable
{
public:
    bool hit(const Ray& ray, const Range& rayRange, Hit& hitData) const override;

    void add(std::shared_ptr<Hittable> obj);

private:
    std::vector<std::shared_ptr<Hittable>> objects;
};

using Mesh = HittableList;
