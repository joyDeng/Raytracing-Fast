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
#include "light.h"
#include "progress.h"

uint64_t primitives_intersected = 0;
uint64_t rays_traced = 0;

Scene::~Scene()
{
    delete m_accelerator;
    delete m_camera;
    for (auto l: m_lights)
        delete l;
    m_lights.clear();
    for (auto m: m_materials)
        delete m.second;
    m_materials.clear();
}

// compute the color corresponing to a ray by raytracing
Color3f Scene::radiance(const Ray3f & ray) const
{
    Intersection3f its;
    if(this->intersect(ray,its)){
        return its.mat->shade(ray, its, *this);
        //Vector3f n = its.sn * 0.5f + Vector3f(0.5f,0.5f,0.5f);
        //return Color3f(0.0f,1.0f,0.0f);
    }
    return m_background;

    // TODO: Assignment 1. In pseudo code:
    //    get scene intersection
    //    if not hit, return background
    //    otherwise, return shade() result of intersection material
}

// raytrace an image
Image3f Scene::raytrace() const
{
    // allocate an image of the proper size
    auto image = Image3f(m_imageWidth, m_imageHeight);
    
    //put_your_code_here("Assignment 1: insert your raytrace() code here");
    
    // TODO: Assignment 1. In pseudo code:
    // if no anti-aliasing
    // foreach image row (go over image height)
    // foreach pixel in the row (go over image width)
				// compute ray-camera parameters (u,v) for the pixel
				// compute camera ray
				// set pixel to the color raytraced with the ray
    
    for(auto y:range(image.height())){
        for(auto x:range(image.width())){
            
            auto ray = m_camera->generateRay((x+0.5f)/m_imageWidth, (y+0.5f)/m_imageHeight);
            image(x,y) = radiance(ray);
        }
    }
    
    // else
    // foreach image row (go over image height)
    // foreach pixel in the row (go over image width)
				// init accumulated color
				// foreach sample in y (go over m_imageSamples)
    //foreach sample in x (go over m_imageSamples)
    // compute ray-camera parameters (u,v) for the sample within the pixel
    // compute camera ray
    // accumulate color raytraced with the ray
				// divide color by the number of samples
    
    
    // done
    
    /*for(auto y:range(image.height())){
     for(auto x:range(image.width())){
     Color3f Accumulatecolor = Color3f(0.0f,0.0f,0.0f);
     auto subpixel = m_imageSamples*m_imageSamples;
     
     for(int i = 0 ; i < m_imageSamples ; i++){
     for(int j = 0 ; j < m_imageSamples ; j++){
     auto ray = m_camera->generateRay((x*m_imageSamples+i+0.5f)/(m_imageWidth*m_imageSamples), (y*m_imageSamples+j+0.5f)/(m_imageHeight*m_imageSamples));
     Accumulatecolor = Accumulatecolor + radiance(ray);
     }
     }
     image(x,y) = Color3f(Accumulatecolor.x()/(subpixel),Accumulatecolor.y()/(subpixel),Accumulatecolor.z()/(subpixel));
     }
     }*/
     
    return image;
}