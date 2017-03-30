/*
    This file is part of Dirt, the Dartmouth introductory ray tracer, used in
    Dartmouth's COSC 77/177 Computer Graphics course.

    Copyright (c) 2016 by Wojciech Jarosz

    Dirt is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License Version 3
    as published by the Free Software Foundation.

    Dirt is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include "scene.h"
#include "light.h"
#include "parser.h"
#include "obj.h"
#include "bbh.h"
#include "surface.h"
#include "sphere.h"
#include "quad.h"
#include <iostream>
#include <Eigen/Geometry>

// local functions
namespace
{

Surface* parseSurface(const Scene & scene, const json & j)
{
    string type;
    try
    {
        type = j.at("type");
    }
    catch (...)
    {
        throw DirtException("Missing \"type\" on surface specification:\n\t%s",
                            j.dump());
    }

    Surface * surface = nullptr;
    if (type == "quad")
        surface = new Quad(scene, j);
    else if (type == "sphere")
        surface = new Sphere(scene, j);
    else if (type == "obj")
        surface = new WavefrontOBJ(scene, j);
    else
        throw DirtException("Unknown surface type \"%s\" in specification:\n\t%s",
                            type.c_str(), j.dump());

    return surface;
}

Accelerator* parseAccelerator(const Scene & scene, const json & j)
{
    if (j.count("accelerator"))
    {
        string type;
        try
        {
            type = j["accelerator"].at("type");
        }
        catch (...)
        {
            throw DirtException("Missing \"type\" on accelerator specification");
        }

        std::transform(type.begin(), type.end(), type.begin(), ::tolower);
        if (type == "bbh" || type == "bvh")
            return new BBH(scene, j["accelerator"]);
        else
            throw DirtException("Unknown accelerator type %s\n", type.c_str());
    }

    // default to a naive accelerator
    return new Accelerator(scene, j);
}

} // namespace


Scene::Scene(const json & j)
{
    parseFromJSON(j);
}

Scene::Scene(const string & filename) :
    Scene(loadJSON(filename))
{
    // empty
}

const Material * Scene::findOrCreateMaterial(const json & j, const Material * defaultMat) const
{
    if (!j.count("material"))
        return defaultMat;

    if (j["material"].is_string())
    {
        // add a pre-declared material
        auto mat = m_materials.find(j["material"]);
        if (mat != m_materials.end())
            return mat->second;
        else
            return defaultMat;
    }
    else if (j["material"].is_object())
        // create a new material
        return new Material(j["material"]);
    else
        return defaultMat;
}

void Scene::parseFromJSON(const json & j)
{
    if (j.empty()) {
        m_accelerator = parseAccelerator(*this, j);
        m_accelerator->build();
        return;
    }
    cout << "parsing..." << endl;
    Parser::get(j, m_imageWidth, "image_width");
    Parser::get(j, m_imageHeight, "image_height");
    Parser::get(j, m_imageSamples, "image_samples");
    Parser::get(j, m_background, "background");

    // create the scene-wide acceleration structure
    m_accelerator = parseAccelerator(*this, j);

    for (auto it = j.begin(); it != j.end(); ++it)
    {
        if (it.key() == "camera")
        {
            if (m_camera)
                throw DirtException("There can only be one camera per scene!");
            m_camera = new Camera(it.value());
        }
        else if (it.key() == "materials")
        {
            for (auto & m : it.value())
            {
                auto material = new Material(m);
                string name;
                try
                {
                    name = m.at("name");
                }
                catch (...)
                {
                    throw DirtException("Missing \"name\" on material specification");
                }
                m_materials[name] = material;
            }
        }
        else if (it.key() == "surfaces")
        {
            for (auto & s : it.value())
            {
                auto surface = parseSurface(*this, s);
                m_accelerator->addSurface(surface);
                m_surfaces.push_back(surface);
            }
        }
        else if (it.key() == "lights")
        {
            for (auto & l : it.value())
                m_lights.push_back(new Light(l));
        }
        else if (it.key() == "image_width")
        {
            m_imageWidth = it.value();
        }
        else if (it.key() == "image_height")
        {
            m_imageHeight = it.value();
        }
        else if (it.key() == "image_samples")
        {
            m_imageSamples = it.value();
        }
        else if (it.key() == "background")
        {
            Parser::get(j, m_background, "background");
        }
        else if (it.key() == "accelerator")
        {
            // already handled above
        }
        else
            throw DirtException("unsupported keyword \"%s\"!", it.key());
    }

    m_accelerator->build();
    message("done parsing scene.\n");
}

Scene* create_test_scene_sphere()
{
    std::string test = R"(
    {
        "camera":
        {
            "transform": { "o": [0,0,2.5] }
        },
        "surfaces": [
            {
                "type": "sphere",
                "material": { "kd": [1,1,1], "ks": [0,0,0], "n": 100 }
            }
        ],
        "lights": [
            {
                "transform": { "o": [0,0,5] },
                "intensity": [10,10,10]
            }
        ],
        "image_samples": 1,
        "image_width": 512,
        "image_height": 512,
        "background": [0.1, 0.1, 0.1]
    }
    )";
    json j = json::parse(test);
    return new Scene(j);
}

Scene* create_test_scene_sphereplane()
{
    std::string test = R"(
    {
        "camera":
        {
            "transform": { "o": [0,0,4] }
        },
        "surfaces": [
            {
                "type": "sphere",
                "radius": 1,
                "material": { "kd": [1,0.75,0.75], "ks": [0,0,0], "n": 100 }
            },
            {
                "type": "quad",
                "transform": { "o": [0,-1,0], "x": [1,0,0], "y": [0,0,-1], "z": [0,1,0] },
                "width": 100,
                "material": { "kd": [1,1,1], "ks": [0,0,0], "n": 100 }
            }
        ],
        "lights": [
            {
                "transform": { "translate": [6,12,6] },
                "intensity": [100,100,100]
            }
        ],
        "image_samples": 1,
        "image_width": 512,
        "image_height": 512,
        "background": [0.2, 0.2, 0.2]
    }
    )";
    json j = json::parse(test);
    return new Scene(j);
}

Scene* create_test_scene_steinbach_screw()
{
    json jobj;
    
    // Compose the camera
    auto transform_obj = json::object({
        {"from", json::array({5.0, 15.0, -25.0})},
        {"to", json::array({0, 0, 0})},
        {"up", json::array({0.0, 1.0, 0.0})}
    });

    jobj["camera"]["transform"] = transform_obj;
    jobj["camera"]["width"] = 6;
    jobj["camera"]["height"] = 6;
    jobj["camera"]["dist"] = 15;

    // compose the image properties
    jobj["image_samples"] = 1;
    jobj["image_width"] = 512;
    jobj["image_height"] = 512;
    jobj["background"] = json::array({ 0.2, 0.2, 0.2 });
    
    // Compose the lights
    jobj["lights"] = json::array({});
    transform_obj = json::object({
        {"o", json::array({0.0, 10.0, -10.0})},
        {"x", json::array({1.0, 0.0, 0.0})},
        {"y", json::array({0.0, 1.0, 0.0})},
        {"z", json::array({0.0, 0.0, 1.0})}
    });

    jobj["lights"] += json::object({
        {"transform", transform_obj},
        {"intensity", json::array({500, 500, 500})}
    });

    jobj["surfaces"] = json::array({});

    Vector3f objectCenter(0.0f, 0.0f, 0.0f);
    float radius = 0.5f;
    int numI = 40;
    int numJ = 40;
    
    for (auto i : range(numI))
    {
        for (auto j : range(numJ))
        {
            float s = (i + 0.5f) / numI;
            float t = (j + 0.5f) / numJ;
            float u = s * (8) - 4.0f;
            float v = t * (6.25f);
            Vector3f center(-u * cos(v), v * cos(u) * 0.75f, u * sin(v));
            auto mat = Material();
            mat.kd = 0.35f*lerp(lerp(Color3f(1.0f, 0.0f, 0.0f), Color3f(0.0f, 1.0f, 0.0f), t),
                                 lerp(Color3f(0.0f, 0.0f, 1.0f), Color3f(0.0f, 0.0f, 0.0f), t), s);
            mat.ks = Color3f(2.0, 2.0, 2.0);
            mat.n = 200;

            auto material_obj = json::object({
                {"kd", json::array({mat.kd.x(), mat.kd.y(), mat.kd.z()})},
                {"ks", json::array({mat.ks.x(), mat.ks.y(), mat.ks.z()})},
                {"n", 200}
            });

            transform_obj = json::object({
                { "o", json::array({ objectCenter.x() + center.x(), objectCenter.y() + center.y(), objectCenter.z() + center.z() }) },
                { "x", json::array({ 1.0, 0.0, 0.0 }) },
                { "y", json::array({ 0.0, 1.0, 0.0 }) },
                { "z", json::array({ 0.0, 0.0, 1.0 }) }
            });

            auto sphere_object = json::object({
                {"type","sphere"},
                {"radius", radius},
                {"transform", transform_obj},
                {"material", material_obj}
            });

            jobj["surfaces"] += sphere_object;
        }
    }
    
    return new Scene(jobj);
}

Scene* create_test_scene(int scene_type)
{
    switch (scene_type)
    {
    case 0: return create_test_scene_sphere();
    case 1: return create_test_scene_sphereplane();
    case 2: return create_test_scene_steinbach_screw();
    }
    error("unknown test scene type %d\n", scene_type);
    return nullptr;
}
