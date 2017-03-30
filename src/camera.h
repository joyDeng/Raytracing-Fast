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

#include "transform.h"
#include "ray.h"
#include "parser.h"

//! This class represents a virtual pinhole camera.
/*!
    The camera is responsible for generating primary rays. It is positioned
    using a Transform and points along the -z axis of the local coordinate
    system. It has an image plane positioned a z = -dist with size
    (width, height). The camera is foc
*/
class Camera
{
public:
    Transform xform = Transform();      //!< local coordinate system
    float width = 1;                    //!< image plane width
    float height = 1;                   //!< image plane height
    float dist = 1;                     //!< image plane distance

    //! Construct a camera from json parameters.
    //
    // The camera setup looks something like this
    //
    //           eye
    //   up <-----+          +
    //           /|\         |
    //          / | \        | dist
    //         /  |  \       |
    //        /   |   \      |
    //       +----|----+     +
    //          height
    //            |
    //            |
    //            |
    //            v
    //
    Camera(const json & j = json())
    {
        Parser::get(j, xform, "transform");
        Parser::get(j, width, "width");
        Parser::get(j, height, "height");
        Parser::get(j, dist, "dist");
    }

    //! Generate a ray going through image-plane location (u,v)
    /*!
        (u,v) range from 0 to 1 along each dimension of the rendered image.
    */
    Ray3f generateRay(float u, float v) const
    {
        // TODO: Assignment 1: Implement camera ray generation
        Vector3f rayv = (Vector3f(-width/2.0f+u*width,height/2.0f-v*height,-dist));
        Point3f  rayp = Point3f(0,0,0);
        Normal3f trayv = (xform * rayv).normalized();
        Point3f  trayp = xform * rayp;
        return Ray3f(trayp, trayv);
    }
};
