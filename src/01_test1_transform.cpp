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

#include "transform.h"

int main(int argc, char** argv)
{
    // PROGRAMMING ASSIGNMENT, PART 1: Transforms revisited
    // ====================================================

    // Create a transform
    Eigen::Matrix4f transformationMatrix;
    transformationMatrix <<
             -0.846852f, -0.492958f, -0.199586f, -0.997497f,
              0.107965f, -0.526819f,  0.843093f,  0.127171f,
             -0.520755f,  0.692427f,  0.499359f, -0.613392f,
                   0.0f,       0.0f,       0.0f,       1.0f;
    Transform transform = Transform(transformationMatrix);

    // Create a normal
    Normal3f normal = Normal3f(-0.281208f, 0.743764f,  0.6064130f);

    // Create a ray with starting point and direction
    Ray3f ray = Ray3f(
        Point3f(-0.997497f, 0.127171f, -0.613392f),
        Vector3f(0.962222f, 0.264941f, -0.0627278f)
    );

    // Check the results
    Normal3f transformedNormal = transform*normal;
    Ray3f transformedRay = transform*ray;

    Point3f  correctTransformedNormal(-0.249534f, 0.0890737f, 0.96426f);
    Point3f  correctRayPosition  =  Point3f(-0.0930302f, -0.564666f, -0.312187f);
    Vector3f correctRayDirection = Vector3f(-0.932945f, -0.088575f, -0.348953f);

    float normalError = (correctTransformedNormal - transformedNormal).cwiseAbs().maxCoeff();
    float rayError = std::max(
        (correctRayPosition  - transformedRay.o).cwiseAbs().maxCoeff(),
        (correctRayDirection - transformedRay.d).cwiseAbs().maxCoeff()
    );

    message("Result of transform*normal:\n%s\n", transformedNormal);
    message("Should be:\n%s\n", correctTransformedNormal);
    if (normalError > 1e-5f)
        warning("Result incorrect!\n\n");
    else
        message("Result correct!\n\n");

    message("transform*ray: transformedRay.o is\n%s\n", transformedRay.o);
    message("(should be\n%s)\n", correctRayPosition);
    message("transform*ray: transformedRay.d is\n%s\n", transformedRay.d);
    message("(should be\n%s)\n", correctRayDirection);
    if (rayError > 1e-5f)
        warning("Result incorrect!\n\n");
    else
        message("Result correct!\n\n");

    return 0;
}
