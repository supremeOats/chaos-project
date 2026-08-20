#pragma once

#include "renderer/Camera.h"
#include "geometry/Mesh.h"
#include "light/Light.h"
#include "light/Light.h"

struct Scene
{
    Camera * camera;
    MeshList * objects;
    LightsList * lights;
    MaterialList * materials;
};
