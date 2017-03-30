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
#include "image.h"

int main(int argc, char** argv)
{
    // PROGRAMMING ASSIGNMENT, PART 1: Transforms
    // ==========================================
    //
    // In this part of the assignment, you will implement transformations of
    // points and vectors. You will implement these in
    // transform.h (look for the empty methods with comments).
    // The first part of this main() will check whether
    // you implemented the methods correctly.

    // To do this, we first create a random vector and point.
    // Note: To create a vector in Dirt with elements (a, b, c), you could also write
    //
    //      auto vector = Vector3f(a, b, c);
    //
    // or
    //
    //      Vector3f vector(a, b, c);
    //
    // and similarly for points. Use whichever form you prefer.

    Vector3f vector = Vector3f(-0.997497f, 0.127171f, -0.6133920f);
    Point3f point   =  Point3f( 0.617481f, 0.170019f, -0.0402539f);


    // Now, we create a transform matrix. To fill matrices in Eigen,
    // we can use the convenient notation of
    //
    //      matrix << a, b, c, d,
    //                e, f, g, h,
    //                i, j, k, l,
    //                m, n, o, p;
    //
    // where white space doesn't matter.
    // Please pay attention to commas and semicolons.

    Eigen::Matrix4f transformationMatrix;
    transformationMatrix <<
             -0.846852f, -0.492958f, -0.199586f, -0.997497f,
              0.107965f, -0.526819f,  0.843093f,  0.127171f,
             -0.520755f,  0.692427f,  0.499359f, -0.613392f,
                   0.0f,       0.0f,       0.0f,       1.0f;



    // Now that we have a transformation matrix, we can create a Dirt transform from it:

    Transform transform = Transform(transformationMatrix);


    // With the transform and the vector/point available, we can multiply them together:

    Vector3f transformedVector = transform*vector;
    Point3f  transformedPoint  = transform*point;


    // Output the results. If you implemented these correctly, both print statements should match.

    Vector3f correctTransformedVector( 0.904467f, -0.6918370f,  0.301205f);
    Point3f  correctTransformedPoint (-1.596190f,  0.0703303f, -0.837324f);

    float vectorError = (correctTransformedVector - transformedVector).cwiseAbs().maxCoeff();
    float  pointError = (correctTransformedPoint  - transformedPoint ).cwiseAbs().maxCoeff();


    message("Result of transform*vector:\n%s\n", transformedVector);
    message("Should be:\n%s\n", correctTransformedVector);
    if (vectorError > 1e-5f)
        warning("Result incorrect!\n\n");
    else
        message("Result correct!\n\n");

    message("Result of transform*point:\n%s\n", transformedPoint);
    message("Should be:\n%s\n", correctTransformedPoint);
    if (pointError > 1e-5f)
        warning("Result incorrect!\n\n");
    else
        message("Result correct!\n\n");


    // PROGRAMMING ASSIGNMENT, PART 2: Image manipulation
    // ==================================================
    //
    // In this part of the assignment you will
    // 1) load an image
    // 2) convert it to grayscale
    // 3) save the modified image

    // First, create the image:
    Image3f image;

    // TODO: Load the image scenes/00_preliminaries/cornellbox.png
    // Hint: Take a look at Image3f::load
    message("Loading image cornellbox.png....\n");

    // TODO: Convert the image to grayscale. Loop over every pixel and convert it to
    // grayscale using the weighted average described in the lecture
    // Hint: Take a look at Image3f::width and Image3f::height
    // Hint: If you have coordinates x and y, you can read a pixel like so: image(x, y)
    // Hint: You can write to pixels in the image using image(x, y) = Color3f(r, g, b);
    message("Converting image to grayscale....\n");

    // TODO: Save the image to scenes/00_preliminaries/cornell_grayscale.png
    // Hint: Take a look at Image3f::save
    message("Saving image cornell_grayscale.png....\n");

    message("Done!\n");
}
