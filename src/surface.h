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

#include "material.h"
#include "transform.h"
#include "ray.h"

//! Superclass for all surfaces
class Surface
{
public:
    Surface(const Scene & scene, const json & j = json());
    virtual ~Surface() {}

    //! Return the total number of primitives in this surface
    virtual uint32_t numPrimitives() const { return 1; }

    //! Return the local-space axis-aligned bounding box containing the given primitive
    virtual Box3f localBBox(uint32_t index) const = 0;

    //! Return the world-space axis-aligned bounding box containing the given primitive
    /*!
        By default just returns the local bounding box transforms by m_xform
    */
    virtual Box3f worldBBox(uint32_t index) const;

    const Material * material() const {return m_material;}
    void setMaterial(Material * m) {delete m_material; m_material = m;}

    //! Ray-Surface intersection test
    virtual bool intersect(uint32_t index, const Ray3f & ray, Intersection3f & its) const = 0;

protected:
    Transform m_xform = Transform();                //!< transformation
    const Material* m_material = new Material();    //!< material
};
