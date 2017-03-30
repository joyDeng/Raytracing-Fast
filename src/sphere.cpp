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

#include "sphere.h"


Sphere::Sphere(const Scene & scene, const json & j)
    : Surface(scene, j)
{
    Parser::get(j, m_radius, "radius");
}

Box3f Sphere::localBBox(uint32_t index) const
{
    return Box3f(-m_radius*Vector3f::Ones(), m_radius*Vector3f::Ones());
}


bool Sphere::intersect(uint32_t, const Ray3f & ray, Intersection3f & its) const
{
    INCREMENT_INTERSECTED_PRIMS;
    // TODO: Assignment 1: Implement ray-sphere intersection
    auto tray = m_xform.inverse()*ray;
    
    auto A = tray.d.dot(tray.d);
    auto B = 2*tray.o.dot(tray.d);
    auto C = tray.o.dot(tray.o) - m_radius*m_radius;
    auto delta = B*B - 4*A*C;
    if(delta < 0)
        return false;
    
    
    auto t1 = (-B + sqrt(delta))/(2*A);
    auto t2 = (-B - sqrt(delta))/(2*A);
    // not hit the surface validly
    //message("%s and %s,%s\n",tray.maxt,t2,t1);
    if(t1 < tray.mint || t2 > tray.maxt)
        return false;
    else if(t2 < tray.mint){
        auto p1 = tray(t1);
        auto gp1 = m_xform * p1;
        Normal3f normal1 = p1 - Point3f::Zero();
        normal1 = (m_xform * normal1).normalized();
        its = Intersection3f(t1,gp1, normal1, normal1,
                             Point2f(gp1.x(),gp1.y()),
                             m_material, this);
        return true;
    }
    else{
        auto p2 = tray(t2);
        auto gp2 = m_xform * p2;
        Normal3f normal2 = p2 - Point3f::Zero();
        normal2 = (m_xform * normal2).normalized();
        its = Intersection3f(t2,gp2, normal2, normal2,
                             Point2f(gp2.x(),gp2.y()),
                             m_material, this);
    }
    
    return true;
}
