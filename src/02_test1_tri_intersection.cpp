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

// runs the raytrace over all tests and saves the corresponding images
int main(int argc, char** argv)
{
    // PROGRAMMING ASSIGNMENT, PART 1: Ray - Triangle Intersection
    // ===========================================================

    message("Testing triangle intersection 1...\n");

    // Test Triangle data
    const Point3f v0(-2.0f, -5.0f, -1.0f), v1(1.0f, 3.0f, 1.0f), v2(2.0f, -2.0f, 3.0);
    const Normal3f n0(0.0f, 0.707106f, 0.707106f),
                   n1(0.666666f, 0.333333f, 0.666666f),
                   n2(0.0f, -0.447213f, -0.894427f);

    // Test ray.
    const Ray3f testRay(Point3f(1.0f, -1.0f, -5.0f), Vector3f(0.0f, 0.20f, 0.50f));
    Intersection3f its;
    
    if (Mesh::singleTriangleIntersect(testRay, v0, v1, v2, &n0, &n1, &n2, its)) {
        
        float correctT = 12.520326f;
        Vector3f correctP(1.0f, 1.504065f, 1.260162f);
        Normal3f correctgN(0.744073f, -0.114473f, -0.658218f);
        Normal3f correctsN(0.762482f, 0.317441f, 0.563784f);

        message("Hit triangle! Distance is %f (should be %f)\n\n", its.t, correctT);
        message("Intersection point is \n%s\n(should be\n%s)\n\n", its.p, correctP);
        message("Geometric normal is \n%s\n(should be \n%s)\n\n", its.gn, correctgN);
        message("Shading normal is \n%s\n(shoudl be \n%s)\n\n", its.sn, correctsN);

        float error = std::max(std::max((correctgN - its.gn).cwiseAbs().maxCoeff(), (correctsN - its.sn).cwiseAbs().maxCoeff()),
            std::max((correctP - its.p).cwiseAbs().maxCoeff(), std::abs(correctT - its.t)));

        if (error > 1e-5f)
            warning("Result incorrect!\n\n");
        else
            message("Result correct!\n\n");
    }
    else {
        warning("Triangle intersection incorrect! Should hit triangle\n");
    }
    
    return 0;
}