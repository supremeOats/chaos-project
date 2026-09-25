#include <sstream>
#include <ctime>
#include <chrono>

#include "utils/Parser.h"


void print_debug_msg(const char* msgStr)
{
    msgStr;
    #ifdef DEBUG
    std::cout << msgStr << '\n';
    #endif    
}

void render_and_mesure_time(const Renderer& renderer, const std::string& outputImageName)
{
    using namespace std::chrono;
    high_resolution_clock::time_point start = high_resolution_clock::now();

    renderer.render(("rendered/" + outputImageName + ".ppm").c_str());

    high_resolution_clock::time_point end = high_resolution_clock::now();
    
    microseconds duration = duration_cast<microseconds>(end - start);    
    double seconds = duration.count() / 1'000'000.0;
    seconds = (std::round(seconds * 100.0)) / 100.0;
    std::cout << "Time elapsed: " << seconds << "s\n";
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

    //Set scene up
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

    //Load scene
    load_scene(sceneData, camera, world, lights, materials, renderer);

    //Render
    render_and_mesure_time(renderer, outputImageName);

    // #ifdef DEBUG
    // render_and_mesure_time(renderer, outputImageName);
    // #else
    // renderer.render(("rendered/" + outputImageName + ".ppm").c_str());
    // #endif

    return 0;
}