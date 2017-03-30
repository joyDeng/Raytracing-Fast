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

#include "surface.h"


//! Superclass for all ray-surface acceleration structures
/*!
    This is the superclass for Accelerators (such as BVHs, KD-Trees) which are
    responsible for performing ray-surface intersection tests against a
    collection of surface primitives. The base class implements a naive
    accelerator which simply intersects each ray with every surface primitive.
    Derived classes could make this faster by using a BVH or KD-Tree.
 */
class Accelerator
{
public:
    //! Create a new and empty naive accelerator
    Accelerator(const Scene & scene, const json & j = json()) {}

    //! Release all resources
    virtual ~Accelerator()
    {
        clear();
    };

    //! Release all resources
    virtual void clear();

    //! Register a surface for inclusion in the Accelerator.
    /*!
        This function can only be used before \ref build() is called
    */
    void addSurface(Surface *surface);

    //! Build the acceleration structure.
    virtual void build();

    //! Intersect a ray against all surfaces registered with the Accelerator
    /*!
        Detailed information about the intersection, if any, will be stored
        in the provided \ref its data record.

        \return \c true If an intersection was found
    */
    virtual bool intersect(const Ray3f & ray, Intersection3f & its) const;

protected:

    //! An adapter for intersecting against a specific primitive within a Surface
    struct Primitive
    {
        const Surface * surface = nullptr;
        uint32_t primitiveIndex = 0;

        Primitive(const Surface * s = nullptr, uint32_t id = 0) :
            surface(s), primitiveIndex(id) {}


        //! Return the local-space axis-aligned bounding box containing the given primitive
        Box3f localBBox() const {return surface->localBBox(primitiveIndex);}
        //! Return the world-space axis-aligned bounding box containing the given primitive
        Box3f worldBBox() const {return surface->worldBBox(primitiveIndex);}

        //! Intersect with the specific primitive in surface
        bool intersect(const Ray3f & ray, Intersection3f & its) const
        {
            return surface->intersect(primitiveIndex, ray, its);
        }
    };

    std::vector<Surface *> m_surfaces;      //!< All surfaces registered with the Accelerator
    std::vector<Primitive *> m_primitives;  //!< All primitives registered with the Accelerator
};
