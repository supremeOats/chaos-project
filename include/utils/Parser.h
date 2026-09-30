#pragma once

#include "geometry/Mesh.h"
#include "renderer/Renderer.h"

#include "rapidjson/document.h"

using namespace rapidjson;

Vector3 read_vec3(const Value& vec);

Color read_color(const Value& col);

RTMatrix read_RTMatrix(const Value& mat);

Material read_material(const Value& material);

void read_material_list(const Value& materials, MaterialList& materialsList);

void read_mesh(const Value& meshData, Mesh& mesh);

void read_objects(const Value& objects, MeshList& world);

void read_settings(const Value& settings, Renderer& renderer);

void read_background(const Value& backgroundData, std::unique_ptr<BaseBG>& background);

void read_camera(const Value& camParams, Camera& cam);

void read_lights(const Value& lightsParams, LightsList& lights);

void load_scene(const Document& scene, Camera& camera, MeshList& world, LightsList& lights, MaterialList& materials, std::unique_ptr<BaseBG>& background, Renderer& renderer);
