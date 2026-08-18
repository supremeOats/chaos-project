#include <sstream>
#include <ctime>
#include <chrono>

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

    #ifdef DEBUG
    std::cout << "\nScene loaded\n";
    #endif
}

int main(int argc, char *argv[])
{
    if (argc != 3) {
        std::cerr << "Scene input file and image output file must be given.";
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

    #ifdef DEBUG
    std::cout << "# of objects: " << scene["objects"].Size() << '\n';
    std::cout << "# of lights: " << lights.size() << '\n';
    std::cout << "# of materials: " << materials.size() << '\n';
    std::cout << "W: " << renderer.width() << "\tH: " << renderer.height() << '\n';
    #endif

    //Single-threaded
    using namespace std::chrono;
    high_resolution_clock::time_point start = high_resolution_clock::now();

    renderer.render(outputImageName.c_str());

    high_resolution_clock::time_point end = high_resolution_clock::now();
    
    microseconds duration = duration_cast<microseconds>(end - start);    
    double seconds = duration.count() / 1'000'000.0;
    std::cout << "Time: " << seconds << "s\n";
    
    #ifdef DEBUG
    std::cout << "\nImage rendered\n";
    #endif

    return 0;
}