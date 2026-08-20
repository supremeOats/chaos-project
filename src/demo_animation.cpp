#include <sstream>
#include <ctime>
#include <chrono>

#include "renderer/Camera.h"
#include "utils/Parser.h"
#include "geometry/Sphere.h"

void animation(const Renderer& renderer, Scene& scene)
{
    const double PI = 3.14159265358979323846;

    const int frameCount = 144;
    for (size_t frame = 0; frame < frameCount; ++frame) {

        if (frame < frameCount/3) {
            scene.camera->set_fov(60 + frame*1.1);
        }
        else {
            scene.camera->turn_table((0.0, 0.0, 0.0), 2*PI/(frameCount*2/3));
        }
        
        for (PointLight& light : *scene.lights) {
            light.turn_table((0.0, 0.0, 0.0), 4*PI/frameCount);
        }

        std::string frameName = "./demo/animation2/frame_" + std::to_string(frame) + ".ppm";

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

    using namespace std::chrono;
    high_resolution_clock::time_point start = high_resolution_clock::now();

    animation(renderer, scene);

    high_resolution_clock::time_point end = high_resolution_clock::now();
    
    microseconds duration = duration_cast<microseconds>(end - start);    
    double seconds = duration.count() / 1'000'000.0;
    std::cout << "Time: " << seconds << "s\n";

    return 0;
}