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
    This file is based on the Mesh class in Nori, a simple educational ray tracer

    Copyright (c) 2015 by Wenzel Jakob, Romain Prévost

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

#include "surface.h"

//! Triangle mesh
/*
    This class stores a triangle mesh object and provides numerous functions
    for querying the individual triangles. Subclasses of \c Mesh implement
    the specifics of how to create its contents (e.g. by loading from an
    external file)
 */
class Mesh : public Surface
{
public:
    //! Return the total number of triangles in this shape
    virtual uint32_t numPrimitives() const override
    {
        return (uint32_t) m_F.size();
    }

    //! Ray-triangle intersection test
    /*
        Performs a ray intersection against a single triangle in the mesh.
        An \ref Accelerator is needed to search for intersections against many
        triangles.

        \param index
            Index of the triangle that should be intersected
        \param ray
            The ray segment to be used for the intersection query
        \param t
            Upon success, \a t contains the distance from the ray origin to the
            intersection point,
        \param u
            Upon success, \c u will contain the 'U' component of the intersection
            in barycentric coordinates
        \param v
            Upon success, \c v will contain the 'V' component of the intersection
            in barycentric coordinates
        \return
            \c true if an intersection has been detected
     */
    virtual bool intersect(uint32_t idx, const Ray3f & ray, Intersection3f & its) const override;

    // Static ray - single triangle intersection routine
    static bool singleTriangleIntersect(const Ray3f& ray, const Vector3f& v0, const Vector3f& v1, const Vector3f& v2,
        const Normal3f* n0, const Normal3f* n1, const Normal3f* n2, Intersection3f& isect);

    //! Return the total number of vertices in this shape
    uint32_t numVertices() const
    {
        return (uint32_t) m_V.size();
    }

    //! Return a reference to the vertex positions
    const std::vector<Point3f> & vertexPositions() const { return m_V; }

    //! Return a reference to the vertex normals
    const std::vector<Normal3f> & vertexNormals() const { return m_N; }

    //! Return a reference to the texture coordinates
    const std::vector<Point2f> & vertexTexCoords() const { return m_UV; }

    //! Return a pointer to the triangle vertex index list
    const std::vector<Vector3i> & faceIndices() const { return m_F; }

    //! Return the local-space axis-aligned bounding box containing the given primitive
    virtual Box3f localBBox(uint32_t index) const override;

protected:
    //! Create an empty mesh
    Mesh(const Scene & scene, const json & j = json());

protected:
    std::vector<Point3f> m_V;           //!< Vertex positions
    std::vector<Normal3f> m_N;          //!< Vertex normals
    std::vector<Point2f> m_UV;          //!< Vertex texture coordinates
    std::vector<Vector3i> m_F;          //!< Faces
};
