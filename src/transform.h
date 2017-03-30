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
/*
    This file is part of Nori, a simple educational ray tracer

    Copyright (c) 2015 by Wenzel Jakob

    Nori is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License Version 3
    as published by the Free Software Foundation.

    Nori is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include "common.h"
#include "vector.h"
#include "box.h"
#include "ray.h"


//! Homogeneous coordinate transformation
/*!
    This class stores a general homogeneous coordinate tranformation, such as
    rotation, translation, uniform or non-uniform scaling, and perspective
    transformations. The inverse of this transformation is also recorded
    here, since it is required when transforming normal vectors.
*/
struct Transform
{
public:
    //! Create the identity transform
    Transform() :
        m_transform(Eigen::Matrix4f::Identity()),
        m_inverse(Eigen::Matrix4f::Identity()) {}

    //! Create a new transform instance for the given matrix
    Transform(const Eigen::Matrix4f &trafo);

    //! Create a new transform instance for the given matrix and its inverse
    Transform(const Eigen::Matrix4f &trafo, const Eigen::Matrix4f &inv)
        : m_transform(trafo), m_inverse(inv) {}

    //! Return the underlying matrix
    const Eigen::Matrix4f & getMatrix() const
    {
        return m_transform;
    }

    //! Return the inverse of the underlying matrix
    const Eigen::Matrix4f & getInverseMatrix() const
    {
        return m_inverse;
    }

    //! Return the inverse transformation
    Transform inverse() const
    {
        return Transform(m_inverse, m_transform);
    }

    //! Concatenate with another transform
    Transform operator*(const Transform &t) const;

    //! Apply the homogeneous transformation to a 3D vector
    Vector3f operator*(const Vector3f &v) const
    {
        // TODO: Assignment 0: Implement this method
        Vector4f v4 = Vector4f(v.x(),v.y(),v.z(),0.0f);
        Vector4f vresult = m_transform*v4;
        Vector3f vr= Vector3f(vresult.x(),vresult.y(),vresult.z());
        
        return vr;
    }

    //! Apply the homogeneous transformation to a 3D normal
    Normal3f operator*(const Normal3f &n) const
    {
        // TODO: Assignment 1: Implement this method
        Vector4f n4 = Vector4f(n.x(),n.y(),n.z(),0.0f);
        Vector4f nresult = m_inverse.transpose()*n4;
        Normal3f nr = (Normal3f(nresult.x(),nresult.y(),nresult.z())).normalized();
        
        return nr;
    }

    //! Transform a point by an arbitrary matrix in homogeneous coordinates
    Point3f operator*(const Point3f &p) const
    {
        // TODO: Assignment 0: Implement this method
        Point4f p4 = Point4f(p.x(),p.y(),p.z(),1.0f);
        Point4f presult = m_transform*p4;
        Point3f pr = Point3f(presult.x(),presult.y(),presult.z());
        
        return pr;
    }

    //! Apply the homogeneous transformation to a ray
    Ray3f operator*(const Ray3f &r) const
    {
        // TODO: Assignment 1: Implement this method
        Vector4f v4 = Vector4f(r.d.x(),r.d.y(),r.d.z(),0.0f);
        Vector4f vresult = m_transform*v4;
        Vector3f v = Vector3f(vresult.x(),vresult.y(),vresult.z());
        
        Point4f p4 = Point4f(r.o.x(),r.o.y(),r.o.z(),1.0f);
        Point4f presult = m_transform*p4;
        Point3f p = Point3f(presult.x(),presult.y(),presult.z());
        
        return Ray3f(p,v.normalized(),r.mint,r.maxt);
    }

    //! Transform the Box and return the resulting bounding box
    Box3f operator*(const Box3f & box) const
    {
        // a transformed empty box is still empty
        if (box.isEmpty())
            return box;

        // Just in case this is a projection matrix, do things the naive way.
        Point3f pts[8];

        // Set up the eight points at the corners of the extent
        pts[0].x() = pts[1].x() = pts[2].x() = pts[3].x() = box.min().x();
        pts[4].x() = pts[5].x() = pts[6].x() = pts[7].x() = box.max().x();

        pts[0].y() = pts[1].y() = pts[4].y() = pts[5].y() = box.min().y();
        pts[2].y() = pts[3].y() = pts[6].y() = pts[7].y() = box.max().y();

        pts[0].z() = pts[2].z() = pts[4].z() = pts[6].z() = box.min().z();
        pts[1].z() = pts[3].z() = pts[5].z() = pts[7].z() = box.max().z();

        Box3f newBox;
        for (int i = 0; i < 8; i++)
            newBox.extend(operator*(pts[i]));

        return newBox;
    }

    //! Return a string representation
    std::string toString() const;

private:
    Eigen::Matrix4f m_transform;
    Eigen::Matrix4f m_inverse;
};
