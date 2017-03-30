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

#include "vector.h"

//! Simple n-dimensional ray segment data structure
/*!
    Along with the ray origin and direction, this data structure additionally
    stores the segment interval [mint, maxt] (whose entries may include positive/
    negative infinity).
*/
template <typename _PointType, typename _VectorType>
struct TRay
{
    typedef _PointType                  PointType;
    typedef _VectorType                 VectorType;
    typedef typename PointType::Scalar  Scalar;

    PointType o;     //!< The origin of the ray
    VectorType d;    //!< The direction of the ray
    Scalar mint;     //!< Minimum distance along the ray segment
    Scalar maxt;     //!< Maximum distance along the ray segment
    int depth;

    //! Construct a new ray
    TRay() : mint(Epsilon),
        maxt(std::numeric_limits<Scalar>::infinity()), depth(0) { }

    //! Construct a new ray
    TRay(const PointType &o, const VectorType &d) : o(o), d(d),
            mint(Epsilon), maxt(std::numeric_limits<Scalar>::infinity()), depth(0) { }

    //! Construct a new ray
    TRay(const PointType &o, const VectorType &d,
        Scalar mint, Scalar maxt) : o(o), d(d), mint(mint), maxt(maxt), depth(0) { }

    //! Copy constructor
    TRay(const TRay &ray)
     : o(ray.o), d(ray.d), mint(ray.mint), maxt(ray.maxt), depth(0) { }

    //! Copy a ray, but change the covered segment of the copy
    TRay(const TRay &ray, Scalar mint, Scalar maxt)
     : o(ray.o), d(ray.d), mint(mint), maxt(maxt), depth(0) { }

    //! Return the position of a point along the ray
    PointType operator() (Scalar t) const { return o + t * d; }

    //! Return a human-readable string summary of this ray
    std::string toString() const
    {
        return tfm::format(
                "Ray[\n"
                "  o = %s,\n"
                "  d = %s,\n"
                "  mint = %f,\n"
                "  maxt = %f\n"
                "]", o.toString(), d.toString(), mint, maxt);
    }
};


//! Contains information about a ray-surface intersection.
/*!
    Used by surface intersection routines in order to return more than just a
    single value like the hit distance. Includes the position, traveled ray
    distance, uv coordinates, the geometric
    and interpolated shading normals, and a pointer to the intersected surface,
    and underlying material.
*/
class Intersection3f
{
public:
    float t;                            //!< Ray parameter for the hit
    Point3f p;                          //!< Hit position
    Normal3f gn;                        //!< Geometric normal
    Normal3f sn;                        //!< Interpolated shading normal
    Point2f uv;                         //!< UV texture coordinates
    const Material* mat = nullptr;      //!< Material at the hit point
    const Surface * surface = nullptr;  //!< Surface at the hit point
    

    Intersection3f()
    {

    }

    Intersection3f(float t,
                   const Point3f &p,
                   const Normal3f &gn,
                   const Normal3f &sn,
                   const Point2f &uv,
                   const Material * m = nullptr,
                   const Surface * s = nullptr) :
        t(t), p(p), gn(gn), sn(sn), uv(uv), mat(m), surface(s)
    {

    }
};
