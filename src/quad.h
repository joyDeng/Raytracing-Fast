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

//! A quad spanning (-m_width, m_width) in the (x,y)-plane at z=0
class Quad : public Surface
{
public:
    Quad(const Scene & scene, const json & j = json());
    virtual Box3f localBBox(uint32_t index) const;
    virtual bool intersect(uint32_t idx, const Ray3f & ray, Intersection3f & its) const;

protected:
    float m_width = 1.0f;
};
