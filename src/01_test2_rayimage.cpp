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

#include "image.h"
#include "camera.h"

// runs the raytrace over all tests and saves the corresponding images
int main(int argc, char** argv)
{
    // PROGRAMMING ASSIGNMENT, PART 2: Ray generation
    // ==============================================

    // First, setup the ray image
    const int RayImageWidth = 1000;
    const int RayImageHeight = 563;
    Image3f rayImage(RayImageWidth, RayImageHeight);

    // Create a transform
    Eigen::Matrix4f transformationMatrix;
    transformationMatrix <<
             -0.846852f, -0.492958f, -0.199586f, -0.997497f,
              0.107965f, -0.526819f,  0.843093f,  0.127171f,
             -0.520755f,  0.692427f,  0.499359f, -0.613392f,
                   0.0f,       0.0f,       0.0f,       1.0f;
    Transform transform = Transform(transformationMatrix);

    // Set up a camera with some reasonable parameters
    Camera camera;
    camera.xform = transform;
    camera.width = RayImageWidth/float(RayImageHeight);
    camera.height = 1.0f;

    // Generate a ray for each pixel in the ray image
    for (auto y : range(RayImageHeight)) {
        for (auto x : range(RayImageWidth)) {
            // Generate the ray through the center of the pixel (hence the +0.5)
            auto ray = camera.generateRay((x + 0.5f)/RayImageWidth, (y + 0.5f)/RayImageHeight);

            // Scale the ray direction into a sensible range for display
            Vector3f scaledDir = ray.d*0.5f + Vector3f(0.5f, 0.5f, 0.5f);
            // Clamp the outputs just in case
            Vector3f clampedDir = scaledDir.cwiseAbs().cwiseMin(1.0f);
            // Write the ray direction to the ray image
            rayImage(x, y) = Color3f(clampedDir[0], clampedDir[1], clampedDir[2]);
        }
    }

    message("Saving ray image to scenes/01_raytrace/rayimage.png....\n\n");
    rayImage.save("scenes/01_raytrace/rayimage.png");

    return 0;
}
