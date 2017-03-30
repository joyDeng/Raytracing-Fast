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

#pragma once

#include "common.h"
#include "image.h"
#include "camera.h"
#include "ray.h"
#include "material.h"
#include "accelerator.h"


//! Main scene data structure
/*!
    This class holds information on scene objects and is responsible for
    coordinating rendering jobs. It also provides useful query routines that
    are mostly used by the \ref Integrator implementations.
*/
class Scene
{
public:
    //! Construct a new scene object
    Scene(const string & filename);
    Scene(const json & j = json());

    //! Release all memory
    virtual ~Scene();

    //! Parser a scene from a json object
    void parseFromJSON(const json & j);

    //! Return a reference to an array containing all lights
    const std::vector<const Light *> &getLights() const { return m_lights; }

    //! Find/create a material
    /*!
        Return a Material pointer by parsing the "material" specification.
        If \a j is a string "material": "material-name", then try to find
        a material with name "material-name" in the pre-declared map of scene
        materials. If \a j is a json object "material": {}, then create a new
        material with the specified parameters.
    */
    const Material * findOrCreateMaterial(const json & j, const Material * def = nullptr) const;

    //! Intersect a ray against all triangles stored in the scene and return detailed intersection information
    /*!
        \param ray
            A 3-dimensional ray data structure with minimum/maximum
            extent information

        \param its
            A detailed intersection record, which will be filled by the
            intersection query

        \return \c true if an intersection was found
    */
    bool intersect(const Ray3f & ray, Intersection3f & its) const
    {
        INCREMENT_TRACED_RAYS;
        return m_accelerator->intersect(ray, its);
    }

    //! Sample the incident radiance along a ray
    /*!
        \param scene
            A pointer to the underlying scene
        \param sampler
            A pointer to a sample generator
        \param ray
            The ray in question
        \return
            An estimate of the radiance in this direction
    */
    Color3f radiance(const Ray3f &ray) const;

    //! Generate the entire image by ray tracing.
    Image3f raytrace() const;

private:
    std::vector<const Surface *> m_surfaces;
    std::map<std::string, const Material *> m_materials;
    Camera * m_camera = nullptr;
    Accelerator * m_accelerator = nullptr;
    std::vector<const Light *> m_lights;
    Color3f m_background = Color3f::Ones()*0.2f;

    int m_imageWidth = 512;                      //!< image resolution in x
    int m_imageHeight = 512;                     //!< image resolution in y
    int m_imageSamples = 1;                      //!< samples per pixels in each direction
};

// create test scenes that do not need to be loaded from a file
Scene* create_test_scene(int scene_type);
