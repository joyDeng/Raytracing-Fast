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

#include "scene.h"
#include "sphere.h"

// runs the raytrace over all tests and saves the corresponding images
int main(int argc, char** argv)
{
    // PROGRAMMING ASSIGNMENT, PART 3: Surface intersection
    // ====================================================

    message("Testing sphere intersection...\n");

    Scene sphereScene;
    Sphere sphere(sphereScene);

    Ray3f sphereTestRay(Point3f(-0.25f, 0.5f, 4.0f), Vector3f(0.0f, 0.0f, -1.0f));
    Intersection3f its;
    if (sphere.intersect(0, sphereTestRay, its)) {
        float correctT = 3.170844f;
        Vector3f correctP(-0.25f, 0.5f, 0.829156f);
        Normal3f correctN(-0.25f, 0.5f, 0.829156f);

        message("Hit sphere! Distance is %f (should be %f)\n", its.t, correctT);
        message("Intersection point is \n%s\n(should be\n%s)\n", its.p, correctP);

        float sphereError = std::max(std::max(
            (correctP - its.p).cwiseAbs().maxCoeff(),
            (correctN - its.p).cwiseAbs().maxCoeff()),
            std::abs(correctT - its.t)
        );

        if (sphereError > 1e-5f)
            warning("Result incorrect!\n\n");
        else
            message("Result correct!\n\n");
    } else {
        warning("Sphere intersection incorrect! Should hit sphere\n");
    }

    return 0;
}
