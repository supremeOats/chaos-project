#include <sstream>
#include <ctime>
#include <chrono>

#include "renderer/Camera.h"
#include "utils/Parser.h"
#include "geometry/Sphere.h"

void load_scene(const Document& scene, Camera& camera, MeshList& world, LightsList& lights, MaterialList& materials, Renderer& renderer)
{
    //Read scene
    read_settings(scene["settings"], renderer);
    
    read_camera(scene["settings"], scene["camera"], camera);
    
    read_objects(scene["objects"], world);
    
    if(scene.HasMember("materials")) {
        read_material_list(scene["materials"], materials);
    } else {
        materials.push_back(Material(DIFFUSE, (1.0), false, 1.0));
    }
    
    if(scene.HasMember("lights")) {
        read_lights(scene["lights"], lights);
    }

    #ifdef DEBUG
    std::cout << "\nScene loaded\n";
    #endif
}

void animation(const Renderer& renderer, Scene& scene)
{
    const double PI = 3.14159265358979323846;

    const int frameCount = 120;
    for (size_t frame = 0; frame < frameCount; ++frame) {

        if (frame < frameCount/3) {
            scene.camera->set_fov(80 + frame*0.5);
        }
        else {
            scene.camera->turn_table((0.0, 0.0, 0.0), 2*PI/frameCount);
        }
        
        for (PointLight& light : *scene.lights) {
            light.turn_table((0.0, 0.0, 0.0), 4*PI/frameCount);
        }

        std::string frameName = "./demo/animation/frame_" + std::to_string(frame) + ".ppm";

        renderer.render(frameName.c_str());

        std::cout << "frame " << frame << " rendered\n";
    }
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
    
    if (!sceneFile.is_open()) {
        std::cerr << "Scene file cannot be opened";
    }

    std::string content(
        (std::istreambuf_iterator<char>(sceneFile)),
        (std::istreambuf_iterator<char>())
    );

    using namespace rapidjson;

    Document sceneData;
    sceneData.Parse(content.c_str());

    //Scene setup
    Camera camera;
    MeshList world;
    LightsList lights;
    MaterialList materials;
    
    Scene scene{
        &camera,
        &world,
        &lights,
        &materials
    };

    Renderer renderer(scene);

    load_scene(sceneData, camera, world, lights, materials, renderer);

    scene.camera->set_fov(100);

    ProceduralSphere sphere(
        Vector3(0.0, 7.0, 0.0),
        3.0
    );
    sphere.set_material(0);

    world.add(std::make_shared<ProceduralSphere>(sphere));

    #ifdef DEBUG
    std::cout << "# of objects: " << scene["objects"].Size() << '\n';
    std::cout << "# of lights: " << lights.size() << '\n';
    std::cout << "# of materials: " << materials.size() << '\n';
    std::cout << "W: " << renderer.width() << "\tH: " << renderer.height() << '\n';
    #endif

    using namespace std::chrono;
    high_resolution_clock::time_point start = high_resolution_clock::now();

    // renderer.render(outputImageName.c_str());
    animation(renderer, scene);

    high_resolution_clock::time_point end = high_resolution_clock::now();
    
    microseconds duration = duration_cast<microseconds>(end - start);    
    double seconds = duration.count() / 1'000'000.0;
    std::cout << "Time: " << seconds << "s\n";

    #ifdef DEBUG
    std::cout << "\nImage rendered\n";
    #endif

    return 0;
}