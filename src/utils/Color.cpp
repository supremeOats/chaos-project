#include "utils/Color.h"

Color norm_vec_to_color(const Vector3& vec)
{
    return Color{
        (unsigned char)(vec.x() * 255),
        (unsigned char)(vec.y() * 255),
        (unsigned char)(vec.z() * 255)
    };
}

Vector3 color_to_norm_vec(const Color& col)
{
    return Vector3(
        (float)col.r / 255,
        (float)col.g / 255,
        (float)col.b / 255
    );
}
