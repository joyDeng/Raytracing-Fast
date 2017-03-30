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

#include "accelerator.h"

// Ray - AABB intersection method
bool aabbIntersect(const Box3f &bounds, const Ray3f &ray, float& tmin, float& tmax);


// BBHAccel Declarations
class BBH: public Accelerator
{
public:
    // BBHAccel Public Methods
    BBH(const Scene & scene, const json & j = json());
    virtual ~BBH();

    virtual void build();

    
    //! Intersect a ray against all surfaces registered with the Accelerator
    virtual bool intersect(const Ray3f & ray, Intersection3f & its) const;
   

    // BBHAccel Private Data
    int maxPrimsInNode = 10;
    enum SplitMethod { SPLIT_MIDDLE, SPLIT_EQUAL_COUNTS, SPLIT_SAH };
    SplitMethod splitMethod = SPLIT_SAH;
    
    struct BBHNode{
        bool isleaf = true;                //node or leaf
        vector<Primitive *> prim;//list of primitive only not empty if node is leaf
        BBHNode *leftchild = nullptr;//only not empty if not lear;
        BBHNode *rightchild = nullptr;//only not empty if not leaf;
        //Primitive leftleaf = nullptr;
        //Primitive rightleaf = nullptr;
        Box3f bounds;//bounding box of the node
    };
    BBH::BBHNode *TreeRoot = nullptr;//root of the BBH
    
    //Create tree node recursively
    BBHNode * CreateNode(std::vector<Primitive *> &A,int type,int AXIS){
        int N = A.size();
        BBHNode *node = new BBHNode;
        
        //if this node is leaf
        if(N <= maxPrimsInNode){
            node->isleaf = true;
            auto it = node->prim.begin();
            node->prim.insert(it,A.begin(),A.begin() + N);
            node->leftchild = nullptr;
            node->rightchild = nullptr;
            node->bounds = A[0]->worldBBox();
            for(auto i:range(N))
                node->bounds = node->bounds.extend(A[i]->worldBBox());
            message("   tree node with %d prim\n",node->prim.size());
            return node;
        }else{
            // if the node is not leaf
            node->isleaf = false;
            //sort along axis
            struct comp{
                int axis;
                bool operator()(Primitive * a,Primitive *b){
                    return a->worldBBox().center()[axis] < b->worldBBox().center()[axis];
                }
                comp(int a){axis = a;}
            };
            std::sort(A.begin(),A.end(),comp(AXIS));
            auto it = A.begin();
            int splite = N/2;
            
            if(type){//if use sah
              Box3f leftbox = A[0]->worldBBox();
              Box3f rightbox = A[1]->worldBBox();
              float SAH = INFINITY;
                //compute area of bbox
                for(auto i:range(N)){
                std::vector<Primitive *> lA(it,it+i);
                std::vector<Primitive *> rA(it + i,it + N);
                for(auto t:range(lA.size()))
                      leftbox = leftbox.extend(lA[t]->worldBBox());
                
                Box3f rightbox = A[i]->worldBBox();
                for(auto j:range(rA.size()))
                    rightbox = rightbox.extend(rA[j]->worldBBox());
                auto s_ah = leftbox.surfaceArea()*lA.size() + rightbox.surfaceArea()*rA.size();
                if(s_ah < SAH){
                    SAH = s_ah;
                    splite = i;
                }
              }
            }
            
            //devide into left and right
            std::vector<Primitive *> LA(it,it+splite);
            std::vector<Primitive *> RA(it+splite,it+N);
            node->leftchild = CreateNode(LA,type,(AXIS+1)%3);
            node->rightchild = CreateNode(RA,type,(AXIS+1)%3);
            Box3f abox = node->leftchild->bounds;
            Box3f bbox = node->rightchild->bounds;
            node->bounds = abox.extend(bbox);
     
            return node;
        }
    }
    
    
  
    bool Recursive(BBHNode *parent,Ray3f & ray,Intersection3f & its)const
    {
        BBHNode * current = parent;
        float minT = ray.mint;
        float maxT = ray.maxt;
        if(aabbIntersect(current->bounds, ray,minT,maxT)){
                if(current->isleaf){
                    //message("hit leaf;index%d !\n",current->prim[0]->primitiveIndex);
                    bool inter = false;
                    float mint = INFINITY;
                    for(auto i:range(current->prim.size())){
                      if(aabbIntersect(current->prim[i]->worldBBox(),ray,minT,maxT)){
                            if(current->prim[i]->intersect(ray, its)){
                                inter = true;
                                if(its.t < mint){
                                    mint = its.t;
                                }
                           }
                        }
                    }
                    if(inter == true) ray.maxt = its.t;
                    return inter;
                }
            
            Intersection3f lits,rits;
            bool left_hit,right_hit;
            left_hit = (current->leftchild != nullptr) && Recursive(current->leftchild,ray,lits);
            right_hit = (current->rightchild != nullptr) && Recursive(current->rightchild,ray,rits);
            if(left_hit && right_hit){
                if(lits.t < rits.t)its = lits;
                else its = rits;
                ray.maxt = its.t;
                return true;
            }else if(right_hit){
                its = rits;
                ray.maxt = its.t;
                return true;
            }else if(left_hit){
                its = lits;
                ray.maxt = its.t;
                return true;
            }else return false;
        }else return false;
    }

};
//BBNode


