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

#include "material.h"
#include "scene.h"
#include "light.h"

Color3f Material::shade(const Ray3f & ray, const Intersection3f & its,
                        const Scene & scene) const
{
    //put_your_code_here("insert your shade() code here");
    
    // Dummy implementation (return surface normal as color)
    // Remove this and write your own
    //Vector3f n = its.gn*0.5f + Vector3f(0.5f, 0.5f, 0.5f);
    //return Color3f(n[0], n[1], n[2]);
    
    // TODO: Assignment 1. In pseudo code:
    // initialize accumulated result to zero
    // foreach light
    // compute light response
    // compute light direction
    // compute the material response (brdf*cos)
    // check for shadows and accumulate to result if not occluded
    // if the material has reflections
    // create the reflection ray
    // accumulate the reflected light (recursive call) scaled by the material reflection
    Color3f Accumulate = Color3f(0.0f,0.0f,0.0f);
    
    vector<const Light *> lights = scene.getLights();
    //message("%s",kd);
    
    for(auto i:range(lights.size())){
        Vector3f l =  (lights[i]->xform)*Point3f(0.0f,0.0f,0.0f) - its.p;//compute light
        Normal3f l_n = l.normalized();//light direction
        
        //if light hit some object on the way to the object
        auto mint = 0.00005f;
        auto maxt = sqrt(l.dot(l)) - 0.00005f;
        Ray3f lightray(its.p,l_n);
        Ray3f lightscale(lightray,mint,maxt);
        Intersection3f l_its;
        
        
        if(scene.intersect(lightscale, l_its)){
            Accumulate += Color3f::Zero();
            //avoid shadow ance
        }
        else{
            //diffuse shading
            Vector3f kdv = Vector3f(kd[0],kd[1],kd[2]);
            Vector3f Ld = Vector3f(kdv[0]*lights[i]->intensity[0],kdv[1]*lights[i]->intensity[1],kdv[2]*lights[i]->intensity[2]) * max(0.0f,l_n.dot(its.sn))/l.dot(l);
            //Boling phong
            Vector3f ksv = Vector3f(ks[0],ks[1],ks[2]);
            //Vector3f h = 2*(its.gn.dot(l_n)*its.gn) - l_n;
            Vector3f h = (l_n - ray.d).normalized();
            Vector3f Ls = Vector3f(ksv[0]*lights[i]->intensity[0],ksv[1]*lights[i]->intensity[1],ksv[2]*lights[i]->intensity[2]) * pow(max(0.0f,its.sn.dot(h)),n)/l.dot(l);
            
            
            
            Accumulate = Accumulate + Color3f(Ld[0],Ld[1],Ld[2])+ Color3f(Ls[0],Ls[1],Ls[2]) ;
        }
        
    }
    //Specular Reflection
    if(ray.depth < 5)
     if(!(kr[0] == 0.0f && kr[1] == 0.0f && kr[2] == 0.0f)){
        Vector3f krv = Vector3f(kr[0],kr[1],kr[2]);
        Normal3f r = (2*its.sn*(its.sn.dot(-ray.d)) + ray.d).normalized();
        Ray3f reflect_ray(its.p,r);
        reflect_ray.depth = ray.depth + 1;
        Color3f trace = scene.radiance(reflect_ray);
        Vector3f trace_color = Vector3f(trace[0],trace[1],trace[2]);
        Vector3f Lr = Vector3f(krv[0] * trace_color[0],krv[1] * trace_color[1],krv[2] * trace_color[2]);
        Accumulate = Accumulate + Color3f(Lr[0],Lr[1],Lr[2]);
     }
    
    //Refraction
    if(ray.depth < 5)
        if(!(kt[0] == 0.0f && kt[1] == 0.0f && kt[2] == 0.0f)){
            auto r = 1/ior;
            Vector3f surface_n = its.sn;
            Vector3f ktv = Vector3f(kt[0],kt[1],kt[2]);
        
            if(ray.d.dot(its.sn) > 0){
             r = ior/1;
             surface_n = -its.sn;
            }
            auto temp = (-ray.d).dot(surface_n);
            Vector3f t = (r*(temp*surface_n + ray.d) - surface_n*sqrt(1 - r*r*(1 - temp*temp))).normalized();
            Ray3f refraction_ray(its.p,t);
            refraction_ray.depth = ray.depth + 1;
            Color3f trace_refra = scene.radiance(refraction_ray);
            Vector3f trace_refra_color = Vector3f(trace_refra[0],trace_refra[1],trace_refra[2]);
            Vector3f Lt = Vector3f(ktv[0] * trace_refra_color[0] , ktv[1] * trace_refra_color[1],ktv[2] * trace_refra_color[2]);
            Accumulate = Accumulate + Color3f(Lt[0],Lt[1],Lt[2]);
        }

    
    return Accumulate;
    
    // If you are taking the glass for graduate credit (177), implement:
    // if the material has refraction
    // create the refraction ray
    // accumulate the refracted light (recursive call) scaled by the material refraction
    
    // return the accumulated color (for now the surface normal)
}
