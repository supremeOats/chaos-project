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
        std::string fName = "results/animation_test2/frame_" + std::to_string(c+1) + ".ppm";
        std::ofstream ppmImage(fName, std::ios::binary | std::ios::trunc);

        if(ppmImage.is_open()) {        
            camera.render(world, ppmImage);
            ppmImage.close();
        }

        camera.dolly(-1.5f/framesCount);
        camera.truck(2.0f/framesCount);
        camera.pan(PI/(4*framesCount));
        //std::cout << "Z: " << camera.pos().z() << '\n';
    }
    
}

int main(int argc, char *argv[])
{
    if (argc > 2) {
        IMAGE_WIDTH = atoi(argv[1]);
        IMAGE_HEIGHT = atoi(argv[2]);
    }

    Camera camera(
        Point3(0, 0, 2),
        Vector3(0, 0, -1),
        35,
        16.0f/9,
        1280
    );
    HittableList world;

    world.add(std::make_shared<ProceduralSphere>(
        Point3(0, 0, -4),
        1.5f
    ));

    world.add(std::make_shared<ProceduralSphere>(
        Point3(-3, 0, -5),
        1.0f
    ));

    world.add(std::make_shared<Plane>(
        Vector3(0, 1, 0),
        Point3(0, 0, 0)
    ));

    animation(IMAGE_HEIGHT, IMAGE_WIDTH, 60, camera, world);

    // //Image
    // std::string fName = "results/image_" + std::to_string(std::time(nullptr)) + ".ppm";
    // std::ofstream ppmImage(fName, std::ios::binary | std::ios::trunc);

    // if(ppmImage.is_open()) {        
    //     camera.render(world, ppmImage);
    //     ppmImage.close();
    // }
}

/*
int main(int argc, char *argv[])
{
    if (argc > 2) {
        IMAGE_WIDTH = atoi(argv[1]);
        IMAGE_HEIGHT = atoi(argv[2]);
    }

    //Triangle
    Triangle t(
        Point3(-0.5, -0.5, -5),
        Point3(0.5, -0.5, -8),
        Point3(0, 0.5, -3)
    );


    Triangle t(
        Point3(-1.75, -1.75, -4),
        Point3(1.75, -1.75, -4),
        Point3(0, 1.75, -4)
    );

    //Image
    std::string fName = "results/image_" + std::to_string(std::time(nullptr)) + ".ppm";
    std::ofstream ppmImage(fName, std::ios::binary | std::ios::trunc);

    if (ppmImage.is_open()) {
        ppmImage << "P6 ";
        ppmImage << IMAGE_WIDTH << ' ' << IMAGE_HEIGHT << ' ';
        ppmImage << MAX_COLOR_COMPONENT << '\n';

        for (size_t row = 0; row < IMAGE_HEIGHT; ++row) {
            for (size_t col = 0; col < IMAGE_WIDTH; ++col) {
                Camera camera;
                
                //Screen space conversion
                float x = col + 0.5;
                x /= IMAGE_WIDTH;
                x -= 0.5;

                float y = row + 0.5;
                y /= IMAGE_HEIGHT;
                y -= 0.5;

                //Current ray
                Ray ray(
                    Point3(0, 0, 0),
                    normalized(Vector3(x, y, -1))
                );

                Vector3 tNorm = t.normal();

                float dist =
                    dot(ray.origin() - t[0], tNorm) /
                    dot(ray.direction(), tNorm);

                //Writing triangle
                if (!parallel(ray.direction(), tNorm)) {
                    Point3 intersectionPoint = ray.at(dist);
                
                    if (t.in_triangle(intersectionPoint)) {
                        ppmImage << RED;
                    }
                    else {
                        ppmImage << WHITE;
                    }
                }
                else {
                    ppmImage << WHITE;
                }
            }
        }
        
        ppmImage.close();
    }
    
    return 0;
}
*/