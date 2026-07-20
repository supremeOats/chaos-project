#include "../include/Triangle.h"
#include "../include/Color.h"
#include "../include/Ray.h"
#include <string>
#include <ctime>
#include <iostream>

int IMAGE_WIDTH = 1920;
int IMAGE_HEIGHT = 1080;
const int MAX_COLOR_COMPONENT = 255;

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

    /*
    Triangle t(
        Point3(-1.75, -1.75, -3),
        Point3(1.75, -1.75, -3),
        Point3(0, 1.75, -3)
    );
    */

    //Image
    std::string fName = "results/image_" + std::to_string(std::time(nullptr)) + ".ppm";
    std::ofstream ppmImage(fName, std::ios::binary | std::ios::trunc);

    if (ppmImage.is_open()) {
        ppmImage << "P6 ";
        ppmImage << IMAGE_WIDTH << ' ' << IMAGE_HEIGHT << ' ';
        ppmImage << MAX_COLOR_COMPONENT << '\n';

        for (size_t row = 0; row < IMAGE_HEIGHT; ++row) {
            for (size_t col = 0; col < IMAGE_WIDTH; ++col) {
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
