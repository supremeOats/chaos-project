#pragma once

#include "renderer/Camera.h"
#include "light/Light.h"

struct Scene
{
    Camera * camera;
    HittableList * objects;
    LightsList * lights;
};
