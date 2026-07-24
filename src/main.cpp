#include "geometry\Triangle.h"
#include "renderer\Camera.h"
#include "geometry\PrimitiveVolumes.h"
#include <string>
#include <ctime>
#include <math.h>
#include <iostream>

int IMAGE_WIDTH = 1920;
int IMAGE_HEIGHT = 1080;

# define PI 3.14159265358979323846

void animation(
    int imgH, int imgW, int framesCount,
    Camera& camera, const HittableList& world)
{
    for (size_t c = 0; c < framesCount; ++c)
    {
        //Current frame
        std::string fName = "results/animation_test3/frame_" + std::to_string(c+1) + ".ppm";
        std::ofstream ppmImage(fName, std::ios::binary | std::ios::trunc);

        if(ppmImage.is_open()) {        
            camera.render(world, ppmImage);
            ppmImage.close();
        }

        camera.pan(2*PI/framesCount);
    }
    
}

int main(int argc, char *argv[])
{
    if (argc > 1) {
        IMAGE_WIDTH = atoi(argv[1]);
    }

    Camera camera(
        Point3(0, 0, 2),
        Vector3(0, 0, -1),
        35,
        16.0f/9,
        IMAGE_WIDTH
    );
    HittableList world;

    world.add(std::make_shared<ProceduralSphere>(
        Point3(-3, 0, -5),
        1.0f
    ));
    
    world.add(std::make_shared<ProceduralSphere>(
        Point3(0, 0, -4),
        1.5f
    ));

    world.add(std::make_shared<Plane>(
        Vector3(0, 1, 0),
        Point3(0, 0, 0)
    ));

    animation(IMAGE_HEIGHT, IMAGE_WIDTH, 72, camera, world);
}