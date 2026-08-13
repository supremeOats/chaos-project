#pragma once

#include "geometry/Mesh.h"
#include "light/Light.h"

struct ShadeContext
{
    Ray const* ray;
    MeshHit const* hitRec;
    MeshList const* objects;
    LightsList const* lights;
    MaterialList const * materials;
};


