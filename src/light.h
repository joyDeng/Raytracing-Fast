#pragma once

//! A class used to represent light sources in a scene.
/*!
    We currently only support point light sources. This class could be made into
    a virtual base class to support other types of lights.
 */
class Light
{
public:
    Transform xform = Transform();          //!< Transformation/position of light
    Color3f intensity = Color3f::Ones();    //!< Intensity

    Light(const json & j = json())
    {
        Parser::get(j, xform, "transform");
        Parser::get(j, intensity, "intensity");
    }
};
