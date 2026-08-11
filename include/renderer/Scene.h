#pragma once

#include "renderer/Camera.h"
#include "light/Light.h"

struct Scene
{
    Camera * camera;
    MeshList * objects;
    LightsList * lights;
};
