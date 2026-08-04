#include "renderer/Camera.h"

const double Camera::RAY_MAX_DIST = 100.0;
Color Camera::BG_COLOR = WHITE;

Camera::Camera(
    const Point3& pos, const Vector3& dir, const float fov,
    const float ratio, const unsigned width)
    : Moveable(pos, dir), _fov(fov), _ratio(ratio), _width(width)
{}

Ray Camera::make_ray(const float x, const float y) const
{
    return Ray(
        pos(),
        normalized(rotation_matrix() * Vector3(x, -y, -0.5f))
    );
}

Color Camera::ray_color(const Ray& ray, const HittableList& world) const
{
    Hit rec;

    if (world.hit(ray, RAY_MAX_DIST, rec) && 0 < rec.t && rec.t < RAY_MAX_DIST) {
        double falloff = 1.0f - rec.t/RAY_MAX_DIST;

        rec.normal *= 255 * falloff;
        return Color{
            (unsigned char) std::abs(rec.normal.x()),
            (unsigned char) std::abs(rec.normal.y()),
            (unsigned char) std::abs(rec.normal.z())
        };
    }
    
    return BG_COLOR;
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
                normalized(rotation_matrix() * Vector3(x, -y, -0.5f))
            );

            imageFile << ray_color(ray, world);
            
        }

        if(row % 100 == 0)
            std::cout << 100 * (float)(row) / (height()) << "%\n";
    }
}