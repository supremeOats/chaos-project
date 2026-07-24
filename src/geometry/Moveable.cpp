#include "geometry\Moveable.h"

void Moveable::dolly(const float dist)
{
    _pos += rotation_matrix() * Vector3(0, 0, dist);
}

void Moveable::truck(const float dist)
{
    _pos += rotation_matrix() * Vector3(dist, 0, 0);
}

void Moveable::pedestal(const float dist)
{
    _pos += rotation_matrix() * Vector3(0, dist, 0);
}

void Moveable::tilt(const float rad)
{
    double arr[9] = {
        1.0f, 0.0f, 0.0f,
        0.0f, std::cos(rad), -1 * std::sin(rad),
        0.0f, std::sin(rad), std::cos(rad)
    };

    Matrix<double, 3, 3> rotationMatrix(arr);

    localCoord = rotationMatrix * localCoord;
}

void Moveable::pan(const float rad)
{
    double arr[9] = {
        std::cos(rad), 0.0f, std::sin(rad),
        0.0f, 1.0f, 0.0f,
        -1 * std::sin(rad), 0.0f, std::cos(rad)
    };

    Matrix<double, 3, 3> rotationMatrix(arr);

    localCoord = rotationMatrix * localCoord;
}

void Moveable::roll(const float rad)
{
    double arr[9] = {
        std::cos(rad), -1 * std::sin(rad), 0.0f,
        std::sin(rad), std::cos(rad), 0.0f,
        0.0f, 0.0f, 1.0f
    };

    Matrix<double, 3, 3> rotationMatrix(arr);

    localCoord = rotationMatrix * localCoord;
}

Vector3 operator*(const RTMatrix& mat, const Vector3& vec)
{
    return Vector3(
        mat.at(0, 0) * vec.x() + mat.at(0, 1) * vec.y() + mat.at(0, 2) * vec.z(),
        mat.at(1, 0) * vec.x() + mat.at(1, 1) * vec.y() + mat.at(1, 2) * vec.z(),
        mat.at(2, 0) * vec.x() + mat.at(2, 1) * vec.y() + mat.at(2, 2) * vec.z()
    );
}

double Moveable::IDENTITY_VALS[9] = {
    1, 0, 0,
    0, 1, 0,
    0, 0, 1
};

RTMatrix Moveable::IDENTITY_MATRIX (IDENTITY_VALS);

