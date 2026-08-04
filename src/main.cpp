#include "renderer/Camera.h"
#include "geometry/PrimitiveVolumes.h"
#include <string>
#include <sstream>
#include <ctime>
#include <math.h>
#include <iostream>

#include "utils/Parser.h"

// int IMAGE_WIDTH = 1920;
// int IMAGE_HEIGHT = 1080;

# define PI 3.14159265358979323846

void animation(int framesCount, Camera& camera, const HittableList& world)
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

        camera.turn_table(Point3(0, 0, -4), 2*PI/framesCount);
    }
    
}

int main(int argc, char *argv[])
{
    std::string fileName = argv[1];

    //Parse scene
    std::ifstream sceneFile("scenes/" + fileName);
    
    std::string content(
        (std::istreambuf_iterator<char>(sceneFile) ),
        (std::istreambuf_iterator<char>()    ) );

    //RapidJSON magic here

    using namespace rapidjson;

    Document scene;
    scene.Parse(content.c_str());

    Camera camera;
    HittableList world;

    // assert(scene.IsObject());
    std::cout << "\n# of objects: " << scene["objects"].Size() << '\n';
    
    read_camera(scene["settings"], scene["camera"], camera);
    camera.pedestal(7);
    camera.tilt(-0.5);
    camera.truck(-2);

    for (SizeType m = 0; m < scene["objects"].Size(); ++m)
    {
        Mesh mesh;
        read_geometry(
            scene["objects"][m]["vertices"],
            scene["objects"][m]["triangles"],
            mesh
        );

        world.add(std::make_shared<Mesh>(mesh));
    }

    std::cout << "\nScene loaded\n";

    std::string fName = "results/scene" + std::to_string(std::time(nullptr)) + ".ppm";
    std::ofstream ppmImage(fName, std::ios::binary | std::ios::trunc);
    
    if(ppmImage.is_open()) {
        std::cout << "W: " << camera.width() << "\tH: " << camera.height() << '\n';

        camera.render(world, ppmImage);
        ppmImage.close();
    }

    std::cout << "\nImage rendered\n";
}