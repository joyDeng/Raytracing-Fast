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

#include "mesh.h"

// Ray-Triangle intersection
// p0, p1, p2 - Triangle vertices
// n0, n1, n2 - optional per vertex normal data
bool Mesh::singleTriangleIntersect(const Ray3f& ray, const Vector3f& p0, const Vector3f& p1, const Vector3f& p2,
    const Normal3f* n0, const Normal3f* n1, const Normal3f* n2, Intersection3f& its)
{
    // TODO: Implement ray-triangle intersection
    
    Vector3f v10 = p1 - p0;
    Vector3f v20 = p2 - p0;
    Vector3f v21 = p2 - p1;
    Vector3f area = v10.cross(v20);
    Normal3f gn = (area).normalized();
    auto t = - (ray.o - p0).dot(gn)/ray.d.dot(gn);
    if(t > ray.maxt || t < ray.mint) return false;
    Point3f x = ray(t);
    //Aproach 1,test direction of cross products
    Vector3f cross_1 = v20.cross(x - p0);
    //message("%f,%f,%f",cross_1[0],cross_1[1],cross_1[2]);
    //message("%f,%f,%f",gn[0],gn[1],gn[2]);
    if(cross_1.dot(gn) > 0) return false;
    Vector3f cross_2 = v10.cross(x - p0);
    if(cross_2.dot(gn) < 0) return false;
    Vector3f cross_3 = v21.cross(x - p1);
    if(cross_3.dot(gn) < 0) return false;
    
    its.t = t;
    its.gn = gn;
    its.p = x;
    
    auto beta = sqrt(cross_2.dot(cross_2)/(area.dot(area)));
    auto gamma = sqrt(cross_1.dot(cross_1)/(area.dot(area)));
    auto alpha = 1 - gamma - beta;
    
    if( n0 != nullptr && n1 != nullptr && n2 != nullptr )//if per vertex normal is avaliable
        its.sn = (alpha * (*n0) + beta * (*n2) + gamma * (*n1)).normalized();
    else its.sn = its.gn;
    its.uv = Point2f(gamma,beta);
    
    
    return true;
}

Mesh::Mesh(const Scene & scene, const json & j) : Surface(scene, j)
{

}

bool Mesh::intersect(uint32_t index, const Ray3f & ray, Intersection3f & its) const
{
    INCREMENT_INTERSECTED_PRIMS;
    // TODO: Implement ray-triangle intersection
    //message("%f,%f,%f,\n",m_V[m_F[index][0]],m_V[m_F[index][1]],m_V[m_F[index][2]]);
    if(singleTriangleIntersect(ray,m_V[m_F[index][0]],m_V[m_F[index][1]],m_V[m_F[index][2]],&m_N[m_F[index][0]],&m_N[m_F[index][1]],&m_N[m_F[index][2]],its)){
          its = Intersection3f(its.t,its.p, its.gn, its.sn,its.uv,m_material, this);
        //message("%f,%f,%f\n",its.gn[0],its.gn[1],its.gn[2]);
          return true;
    }
    return false;
}

Box3f Mesh::localBBox(uint32_t index) const
{
    Box3f result;
    result.extend(m_xform.inverse() * m_V[m_F[index][0]]);
    result.extend(m_xform.inverse() * m_V[m_F[index][1]]);
    result.extend(m_xform.inverse() * m_V[m_F[index][2]]);
    return result;
}
