#include "renderer/Renderer.h"

void Renderer::render(std::ofstream& imageFile) const
{
    imageFile << "P6 ";
    imageFile << _width << ' ' << _height << ' ';
    imageFile << MAX_COLOR_COMPONENT << '\n';

    for (size_t row = 0; row < _height; ++row) {
        for (size_t col = 0; col < _width; ++col) {
            float x = col + 0.5;
            x /= _width;
            x -= 0.5;
            x *= ratio();

            float y = row + 0.5;
            y /= _height;
            y -= 0.5;

            Ray ray = scene.camera->make_ray(x, y);

            imageFile << scene.camera->ray_color(ray, *scene.objects, *scene.lights);
        }

        if(row % 100 == 0)
            std::cout << 100 * (float)(row) / (_height) << "%\n";
    }
}
