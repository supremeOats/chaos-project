#include "renderer\Camera.h"

Camera::Camera(
    const Point3& pos, const Vector3& dir, const float fov,
    const float ratio, const unsigned width)
    : Moveable(pos, dir), _fov(fov), _ratio(ratio), _width(width)
{
    std::cout << "W: " << this->width() << "\tH: " << this->height() << '\n';
    std::cout << "Ratio: " << _ratio << '/n';
}

Color Camera::ray_color(const Ray& ray, const HittableList& world) const
{
    Hit rec;

    if (world.hit(ray, rec)) {
        rec.normal *= 255 - rec.t*5;
        return Color{
            (unsigned char) std::abs(rec.normal.x()),
            (unsigned char) std::abs(rec.normal.y()),
            (unsigned char) std::abs(rec.normal.z())
        };
    }
    
    return WHITE;
}

void Camera::render(const HittableList& world, std::ofstream& imageFile) const
{
    imageFile << "P6 ";
    imageFile << width() << ' ' << height() << ' ';
    imageFile << MAX_COLOR_COMPONENT << '\n';

    for (size_t row = 0; row < height(); ++row) {
        for (size_t col = 0; col < width(); ++col) {
            float x = col + 0.5;
            x /= width();
            x -= 0.5;
            x *= ratio();

            float y = row + 0.5;
            y /= height();
            y -= 0.5;

            Ray ray(
                pos(),
                normalized(rotation_matrix() * Vector3(x, y, -1))
            );

            imageFile << ray_color(ray, world);
        }
    }
}