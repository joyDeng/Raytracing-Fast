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

#include "surface.h"
#include "scene.h"
#include <Eigen/Geometry>

Surface::Surface(const Scene & scene, const json & j)
{
    Parser::get(j, m_xform, "transform");
    m_material = scene.findOrCreateMaterial(j, m_material);
}

Box3f Surface::worldBBox(uint32_t index) const
{
    return m_xform * localBBox(index);
}
