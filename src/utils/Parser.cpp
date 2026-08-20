#include "utils/Parser.h"

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
    return norm_vec_to_color(v);
}

RTMatrix read_RTMatrix(const Value& mat)
{
    RTMatrix res;
    
    for (size_t i = 0; i < 3; ++i) {
        for (size_t j = 0; j < 3; ++j) {
            SizeType matIdx = (SizeType)i*3 + (SizeType)j;
            res.at(i, j) = mat[matIdx].GetDouble();
        }
    }
    
    return res;
}

Material read_material(const Value& material)
{
    MaterialType type;

    //TODO - replace with switch-case
    if (material["type"] == "diffuse") {
        type = MaterialType::DIFFUSE;
    } else if (material["type"] == "reflective") {
        type = MaterialType::REFLECTIVE;
    } else if (material["type"] == "refractive") {
        type = MaterialType::REFRACTIVE;
    } else  if (material["type"] == "constant") {
        type = MaterialType::CONSTANT;
    } else {
        type = MaterialType::CONSTANT;
    }

    Vector3 albedo = material.HasMember("albedo") ? read_vec3(material["albedo"]) : Vector3(1.0);
    bool smooth = material.HasMember("smooth_shading") ? material["smooth_shading"].GetBool() : false;
    double ior = material.HasMember("ior") ? material["ior"].GetDouble() : 1.0;

    return Material(
        type,
        albedo,
        smooth,
        ior
    );
}

void read_material_list(const Value& materials, MaterialList& materialsList)
{
    for (SizeType m = 0; m < materials.Size(); ++m) {
        materialsList.push_back(read_material(materials[m]));
    }
}

void read_mesh(const Value& meshData, Mesh& mesh)
{
    const Value& verts = meshData["vertices"];
    const Value& triangles = meshData["triangles"];

    if(meshData.HasMember("material_index")) {
        mesh.set_material(meshData["material_index"].GetInt());
    } else {
        mesh.set_material(0);
    }

    for (SizeType i = 0; i < verts.Size(); i += 3) {
        mesh.add_vert(Point3(
            verts[i].GetDouble(),
            verts[i+1].GetDouble(),
            verts[i+2].GetDouble()
        ));
    }

    for (SizeType t = 0; t < triangles.Size(); t += 3) {
        unsigned idxV0 = triangles[t].GetUint();
        unsigned IdxV1 = triangles[t + 1].GetUint();
        unsigned IdxV2 = triangles[t + 2].GetUint();

        mesh.add_tri(MeshTriangle(idxV0, IdxV1, IdxV2));
    }

    mesh.update_vert_normals();
    mesh.update_aabb();
}

void read_objects(const Value& objects, MeshList& world)
{
    for (SizeType m = 0; m < objects.Size(); ++m)
    {
        Mesh mesh;
        read_mesh(
            objects[m],
            mesh
        );

        world.add(std::make_shared<Mesh>(mesh));
    }
}

void read_settings(const Value& settings, Renderer& renderer)
{
    renderer.set_dimentions(
        settings["image_settings"]["width"].GetUint(),
        settings["image_settings"]["height"].GetUint()
    );
}

void read_camera(const Value& settings, const Value& camParams, Camera& cam)
{
    if (settings.HasMember("background_color"))
        cam.BG_COLOR = read_color(settings["background_color"]);
    
    Point3 pos = read_vec3(camParams["position"]);
    cam.move_to(pos);

    cam.set_rotation(read_RTMatrix(camParams["matrix"]));
}

void read_lights(const Value& lightsParams, LightsList& lights)
{
    for (SizeType i = 0; i < lightsParams.Size(); ++i) {
        lights.push_back(PointLight(
            read_vec3(lightsParams[i]["position"]),
            (lightsParams[i].HasMember("color")) ? read_color(lightsParams[i]["color"]) : WHITE,
            lightsParams[i]["intensity"].GetFloat()
        ));
    }    
}

void load_scene(const Document& scene, Camera& camera, MeshList& world, LightsList& lights, MaterialList& materials, Renderer& renderer)
{
    read_settings(scene["settings"], renderer);
    
    read_camera(scene["settings"], scene["camera"], camera);
    
    read_objects(scene["objects"], world);
    
    if(scene.HasMember("materials")) {
        read_material_list(scene["materials"], materials);
    } else {
        materials.push_back(Material(DIFFUSE, (1.0), false, 1.0));
    }
    
    if(scene.HasMember("lights")) {
        read_lights(scene["lights"], lights);
    }

    #ifdef DEBUG
    std::cout << "\nScene loaded\n";
    #endif
}
