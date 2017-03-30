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

#include "bbh.h"

// runs the raytrace over all tests and saves the corresponding images if any
int main(int argc, char** argv)
{
    // PROGRAMMING ASSIGNMENT - 2, PART 2: Ray - AABB intersection
    // ===========================================================

    message("Testing Ray - AABB intersection...\n");

    Box3f bbox(Vector3f(-2.0f, -2.0f, 1.0f), Vector3f(2.0f, 2.0f, 5.0f));
    Ray3f aabbTestRay(Point3f(-0.5f, -1.0f, -5.0f), Vector3f(0.0f, 0.0f, 1.0f));
    float minT, maxT;

    // Correct expected output
    float correct_minT = 6.0f;
    float correct_maxT = 10.0f;
    Point3f correct_nearPt(-0.50f, -1.0f, 1.0f);

    if (aabbIntersect(bbox, aabbTestRay, minT, maxT)) {
        
        // Compute entry point into the AABB
        Point3f nearPt = aabbTestRay(minT);
        
        message("Hit AABB! minT is %f (should be %f)\n", minT, correct_minT);
        message("Hit AABB! maxT is %f (should be %f)\n", maxT, correct_maxT);
        message("Hit AABB! nearest hit point is \n%s\n(should be\n%s)\n", nearPt, correct_nearPt);
        
        // Check for errors in the minT, maxT and nearest intersection point
        float error = std::max(std::max(std::abs(correct_maxT - maxT), std::abs(correct_minT - minT)),
            (correct_nearPt - nearPt).cwiseAbs().maxCoeff());

        if (error > 1e-5f)
            warning("Result incorrect!\n\n");
        else
            message("Result correct!\n\n");
    }
    else {
        warning("AABB intersection incorrect! Should hit AABB\n");
    }

    return 0;
}