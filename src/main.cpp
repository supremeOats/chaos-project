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

void animation(int framesCount, Renderer& renderer)
{
    for (size_t c = 0; c < framesCount; ++c)
    {
        //Current frame
        std::string fName = "results/animation_test3/frame_" + std::to_string(c+1) + ".ppm";
        std::ofstream ppmImage(fName, std::ios::binary | std::ios::trunc);

        if(ppmImage.is_open()) {        
            renderer.render(ppmImage);
            ppmImage.close();
        }

        // camera.turn_table(Point3(0, 0, -4), 2*PI/framesCount);
    }
    
}

int main(int argc, char *argv[])
{
    std::string fileName = argv[1];

    //Parse scene
    std::ifstream sceneFile("scenes/" + fileName);
    
    std::string content(
        (std::istreambuf_iterator<char>(sceneFile)),
        (std::istreambuf_iterator<char>()) 
    );

    using namespace rapidjson;

    Document scene;
    scene.Parse(content.c_str());

    //Scene setup
    Camera camera;
    HittableList world;
    LightsList lights;
    
    Renderer renderer(Scene{
        &camera,
        &world,
        &lights
    });

    //Reading scene
    read_settings(scene["settings"], renderer);
    read_camera(scene["settings"], scene["camera"], camera);
    read_objects(scene["objects"], world);

    if(scene.HasMember("lights")) {
        read_lights(scene["lights"], lights);
    }

    std::cout << "\nScene loaded\n";

    std::cout << "# of objects: " << scene["objects"].Size() << '\n';
    std::cout << "# of lights: " << scene["lights"].Size() << '\n';

    //File creation & rendering
    std::string fName = "results/scene.ppm";
    std::ofstream ppmImage(fName, std::ios::binary | std::ios::trunc);
    
    if(ppmImage.is_open()) {
        std::cout << "W: " << renderer.width() << "\tH: " << renderer.height() << '\n';

        renderer.render(ppmImage);
        ppmImage.close();
    }

    std::cout << "\nImage rendered\n";
}