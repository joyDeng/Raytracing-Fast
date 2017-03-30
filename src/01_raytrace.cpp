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
#include "parser.h"

// runs the raytrace over all tests and saves the corresponding images
int main(int argc, char** argv)
{
    // PROGRAMMING ASSIGNMENT, PART 4/5: Ray tracing & Shading
    // =======================================================

    auto args = parseCmdline(argc, argv,
    {
        "01_raytrace", "raytrace a scene", {},
        {
            {"scene_filename", "",  "scene filename",   typeid(string), false, json("scene.json")},
            {"image_filename", "",  "image filename",   typeid(string), true,  json("")}
        }
    });

    // generate/load scene either by creating a test scene or loading from json file
    string scene_filename = args["scene_filename"];
    Scene *scene = nullptr;
    try
    {
        if (scene_filename.length() > 9 and scene_filename.substr(0,9) == "testscene")
        {
            int scene_type = atoi(scene_filename.substr(9).c_str());
            scene = create_test_scene(scene_type);
            scene_filename = scene_filename + ".json";
        }
        else
        {
            scene = new Scene(scene_filename);
        }
    }
    catch (const std::exception & e)
    {
        error("Could not create scene: %s.\n", e.what());
        exit(EXIT_FAILURE);
    }

    auto image_filename = (args["image_filename"].get<string>() != string("")) ?
                           args["image_filename"].get<string>() :
                           scene_filename.substr(0, scene_filename.size()-5)+".png";

    auto image = scene->raytrace();
    
    message("\nAverage Ray-Primitive Intersections : %f \n", static_cast<float>(primitives_intersected) / static_cast<float>(rays_traced));
    message("writing to png...\n");
    image.save(image_filename);

    delete scene;
    message("done\n");
}
