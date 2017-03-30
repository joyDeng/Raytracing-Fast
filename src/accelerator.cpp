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

#include "accelerator.h"

void Accelerator::addSurface(Surface *surface)
{
    m_surfaces.push_back(surface);

    // add all the primitives within surface to the list of primitives
    for (int prim : range(surface->numPrimitives()))
        m_primitives.push_back(new Primitive(surface, prim));
}

void Accelerator::clear()
{
    for (auto surface : m_surfaces)
        delete surface;
    m_surfaces.clear();
    m_surfaces.shrink_to_fit();

    for (auto primitive : m_primitives)
        delete primitive;
    m_primitives.clear();
    m_primitives.shrink_to_fit();
}

void Accelerator::build()
{
    // nothing to do here for naive accelerator
}

bool Accelerator::intersect(const Ray3f & _ray, Intersection3f & its) const
{
    // copy the ray so we can modify the tmax values as we traverse
    Ray3f ray = _ray;
    bool hitSomething = false;
    
    // This is a linear intersection test that iterates over all primitives
    // within the scene. It's the most naive intersection test and hence very slow.

    // foreach primitive
    for (auto primitive : m_primitives)
    {
        if (primitive->intersect(ray, its))
        {
            hitSomething = true;
            ray.maxt = its.t;
        }
    }

    // record closest intersection
    return hitSomething;
}
