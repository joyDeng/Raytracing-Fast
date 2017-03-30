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

bool aabbIntersect(const Box3f &bounds, const Ray3f &ray, float& minT, float& maxT)
{
    // TODO: Assignment 2: Implement ray-AABB intersection
    //auto bound = m_xform.inverse() * bounds;
    Normal3f d = ray.d.normalized();
    float a[3] = {1/d[0],1/d[1],1/d[2]};
    Point3f TLC = Point3f(bounds.corner(bounds.TopLeftCeil));
    //Point3f TRC = bounds.corner(bounds.TopRightCeil);
    //Point3f BLC = bounds.corner(bounds.BottomLeftCeil);
    //Point3f BRC = bounds.corner(bounds.BottomRightCeil);
    Point3f TLF = Point3f(bounds.corner(bounds.TopLeftFloor));
    //Point3f TRF = bounds.corner(bounds.TopRightFloor);
    Point3f BLF = Point3f(bounds.corner(bounds.BottomLeftFloor));
    Point3f BRF = Point3f(bounds.corner(bounds.BottomRightFloor));
    Vector3f tmin = Vector3f(0.0f,0.0f,0.0f);
    Vector3f tmax = Vector3f(0.0f,0.0f,0.0f);
     if(a[0] > 0){
        tmin[0] = a[0] * (BLF[0] - ray.o[0]);
        tmax[0] = a[0] * (BRF[0] - ray.o[0]);
       }else{
        tmin[0] = a[0] * (BRF[0] - ray.o[0]);
        tmax[0] = a[0] * (BLF[0] - ray.o[0]);
      }
    
    if(a[1] > 0){
        tmin[1] = a[1] * (BLF[1] - ray.o[1]);
        tmax[1] = a[1] * (TLF[1] - ray.o[1]);
    }else{
        tmax[1] = a[1] * (BLF[1] - ray.o[1]);
        tmin[1] = a[1] * (TLF[1] - ray.o[1]);
    }
    
    if(a[2] > 0){
        tmin[2] = a[2] * (TLF[2] - ray.o[2]);
        tmax[2] = a[2] * (TLC[2] - ray.o[2]);
    }else{
        tmax[2] = a[2] * (TLF[2] - ray.o[2]);
        tmin[2] = a[2] * (TLC[2] - ray.o[2]);
    }
    
    
    minT = max(max(tmin[0],tmin[1]),tmin[2]);
    maxT = min(min(tmax[0],tmax[1]),tmax[2]);
    
    return minT < maxT;
}

BBH::BBH(const Scene & scene, const json & j) : Accelerator(scene, j)
{
    Parser::get(j, maxPrimsInNode, "maxPrimsInNode");
    maxPrimsInNode = std::min(255, maxPrimsInNode);
    string sm("sah");
    Parser::get(j, sm, "splitMethod");

    // NOTE: If you are a GRAD student taking the course, you are required to implement
    //         1. SAH split method
    //         2. Either middle or equal splitting method or both
    if (sm == "sah")
        splitMethod = SPLIT_SAH;

    // NOTE: If you are a UNDERGRAD student taking the course, you are required to implement
    //         1. Both middle and equal splitting method
    //         2. SAH method for extra credit
    else if (sm == "middle")
        splitMethod = SPLIT_MIDDLE;
    else if (sm == "equal")
        splitMethod = SPLIT_EQUAL_COUNTS;
    else
    {
        warning("BBH split method \"%s\" unknown. Using \"sah\".", sm.c_str());
        splitMethod = SPLIT_SAH;
    }
}

            
void BBH::build()
{
    message("building a %s BBH with max %d primitives per node...\n", splitMethod == SPLIT_MIDDLE ? "split-middle" : (splitMethod == SPLIT_EQUAL_COUNTS ? "split-equal-count" : "surface-area-heuristic"), maxPrimsInNode);

    // PSEUDO CODE
    // ***********
    //
    // Increment total nodes that are created so far
    // Create a root as new node and push into the list of nodes to process
    // If #primitives < maxPrimsInNode
        // Create a leaf node
    // Else
        // Choose split dimension
        // Partition primitives into two sets and build children
        // Recursively call the function on both children
    put_your_code_here("Assignment 2: Insert your BBH::build() code here \n");
    int type = 0;
    if(this->splitMethod == this->SPLIT_MIDDLE)
        type = 1;
    
    message("There are: %d primitives",m_primitives.size());
    BBHNode *node = CreateNode(m_primitives,type,0);
    this->TreeRoot = node;
    
    
    message("root is here");
    
    
    
        
    
}

BBH::~BBH()
{
}


bool BBH::intersect(const Ray3f & _ray, Intersection3f & its) const
{
    // PSEUDO CODE
    // ***********
    //
    // Make root node current node
    // If intersects current node AABB
        // If intersects left child AABB
            // If left node == internal node
                // Make current node = left node
                // Recurse on current node
            // Else if node == leaf
                // Find intersection with primitives and update ray params
        // If intersects right child AABB
            // If right node == internal node
                // Make current node = right node
                // recurse on current node
            // Else if node == leaf
                // Find intersecction with primitives and update ray params
    // Else
        // return
    BBHNode *current = this->TreeRoot;
    Ray3f ray = _ray;

    if(Recursive(current,ray,its)) return true;
    return false;
    
   
}
