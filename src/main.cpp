#include <sstream>
#include <ctime>
#include <math.h>

#include "renderer/Camera.h"
#include "utils/Parser.h"

void load_scene(const Document& scene, Camera& camera, MeshList& world, LightsList& lights, MaterialList& materials, Renderer& renderer)
{
    //Read scene
    read_settings(scene["settings"], renderer);
    read_camera(scene["settings"], scene["camera"], camera);
    read_objects(scene["objects"], world);
    read_material_list(scene["materials"], materials);

    if(scene.HasMember("lights")) {
        read_lights(scene["lights"], lights);
    }

    std::cout << "\nScene loaded\n";
}

int main(int argc, char *argv[])
{
    if (argc != 3) {
        std::cout << "Scene input file and image output file must be given.";
        return 1;
    }

    std::string sceneFileName = argv[1];
    std::string outputImageName = argv[2];

    //Parse scene
    std::ifstream sceneFile("scenes/" + sceneFileName);
    
    std::string content(
        (std::istreambuf_iterator<char>(sceneFile)),
        (std::istreambuf_iterator<char>())
    );

    using namespace rapidjson;

    Document scene;
    scene.Parse(content.c_str());

    //Scene setup
    Camera camera;
    MeshList world;
    LightsList lights;
    MaterialList materials;
    
    Renderer renderer(Scene{
        &camera,
        &world,
        &lights,
        &materials
    });

    load_scene(scene, camera, world, lights, materials, renderer);

    std::cout << "# of objects: " << scene["objects"].Size() << '\n';
    std::cout << "# of lights: " << lights.size() << '\n';
    std::cout << "# of materials: " << materials.size() << '\n';

    //File creation & rendering
    std::ofstream ppmImage(outputImageName, std::ios::binary | std::ios::trunc);
    
    if(ppmImage.is_open()) {
        std::cout << "W: " << renderer.width() << "\tH: " << renderer.height() << '\n';

        renderer.render(ppmImage);
        ppmImage.close();
    } else {
        std::cout << "Output file cannot be opened";
        return 1;
    }

    std::cout << "\nImage rendered\n";

    return 0;
}