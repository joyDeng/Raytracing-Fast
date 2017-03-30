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

#include "quad.h"

Quad::Quad(const Scene & scene, const json & j)
    : Surface(scene, j)
{
    Parser::get(j, m_width, "width");
}

bool Quad::intersect(uint32_t, const Ray3f & ray, Intersection3f & its) const
{
    INCREMENT_INTERSECTED_PRIMS;

    // compute ray intersection (and ray parameter), continue if not hit
    auto tray = m_xform.inverse() * ray;
    if (tray.d.z() == 0)
        return false;
    auto t = -tray.o.z() / tray.d.z();
    auto p = tray(t);
    if (m_width < p.x() || -m_width > p.x() || m_width < p.y() || -m_width > p.y())
        return false;

    // check if computed param is within ray.mint and ray.maxt
    if (t < tray.mint || t > tray.maxt)
        return false;

    Normal3f gn = (m_xform * Normal3f(0,0,1)).normalized();
    // if hit, set intersection record values
    its = Intersection3f(t, m_xform * p, gn, gn,
                         Point2f(lerpFactor(-m_width, m_width, p.x()),
                                 lerpFactor(-m_width, m_width, p.y())),
                         m_material, this);
    return true;
}


Box3f Quad::localBBox(uint32_t index) const
{
    return Box3f(-m_width*Vector3f(1,1,0), m_width*Vector3f(1,1,0));
}
