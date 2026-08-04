#pragma once

#include "geometry/Triangle.h"
#include "renderer/Camera.h"
#include "Color.h"

#include "rapidjson/document.h"

using namespace rapidjson;

Vector3 read_vec3(const Value& vec)
{
    return Vector3(
        vec[0].GetDouble(),
        vec[1].GetDouble(),
        vec[2].GetDouble()
    );
}

Color read_color(const Value& col)
{
    Vector3 v = read_vec3(col);
    return double_to_8bit(v);
}

RTMatrix read_RTMatrix(const Value& mat)
{
    RTMatrix res;

    for (size_t i = 0; i < 3; ++i) {
        for (size_t j = 0; j < 3; ++j) {
            res.at(i, j) = mat[i*3 + j].GetDouble();
        }
    }
 
    return res;
}

void read_geometry(const Value& verts, const Value& vertIndices, Mesh& mesh)
{
    std::vector<Point3> vertArr; //(verts.Size() / 3);
    for (size_t i = 0; i < verts.Size(); i += 3) {
        vertArr.push_back(Vector3(
            verts[i].GetDouble(),
            verts[i+1].GetDouble(),
            verts[i+2].GetDouble()
        ));
    }

    for (size_t t = 0; t < vertIndices.Size(); t += 3) {
        unsigned idxV0 = vertIndices[t].GetUint();
        unsigned IdxV1 = vertIndices[t + 1].GetUint();
        unsigned IdxV2 = vertIndices[t + 2].GetUint();

        Triangle curr(
            vertArr[idxV0],
            vertArr[IdxV1],
            vertArr[IdxV2]
        );

        mesh.add(std::make_shared<Triangle>(curr));
    }
}

void read_camera(const Value& settings, const Value& camParams, Camera& cam)
{
    //Settings
    if (settings.HasMember("background_color"))
        cam.BG_COLOR = read_color(settings["background_color"]);

    cam.set_width(settings["image_settings"]["width"].GetUint());
    cam.set_height(settings["image_settings"]["height"].GetUint());

    //Params
    Point3 pos = read_vec3(camParams["position"]);
    cam.move_to(pos);

    cam.set_rotation(read_RTMatrix(camParams["matrix"]));
}

