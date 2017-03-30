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

#if defined(_MSC_VER)
/* Disable some warnings on MSVC++ */
#pragma warning(disable : 4127 4702 4100 4515 4800 4146 4512)
#define WIN32_LEAN_AND_MEAN     /* Don't ever include MFC on Windows */
#define NOMINMAX                /* Don't override min/max */
#endif

#include <string>
#include <vector>
#include <map>
#include <set>
#include <cassert>
#include <stdio.h>
#include <cstdarg>
#include <fstream>
#include <iostream>
#include <cstdio>
#include <typeinfo>
#include <tinyformat.h>

#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif
#include <Eigen/Core>
#include <Eigen/Geometry>
#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif

// bringing stand libraray objects in scope
using std::string;
using std::vector;
using std::map;
using std::pair;
using std::make_pair;
using std::set;
using std::cout;
using std::cin;
using std::cerr;
using std::endl;


// "Ray epsilon": relative error threshold for ray intersection computations
#define Epsilon 1e-4f

// A few useful constants
#undef M_PI

#define M_PI         3.14159265358979323846f
#define INV_PI       0.31830988618379067154f
#define INV_TWOPI    0.15915494309189533577f
#define INV_FOURPI   0.07957747154594766788f
#define SQRT_TWO     1.41421356237309504880f
#define INV_SQRT_TWO 0.70710678118654752440f


// math utilities --------------------------
// max
inline int max(int a, int b)
{
    return (a > b) ? a : b;
}
inline float max(float a, float b)
{
    return (a > b) ? a : b;
}
// min
inline int min(int a, int b)
{
    return (a < b) ? a : b;
}
inline float min(float a, float b)
{
    return (a < b) ? a : b;
}
// clamp (sets x between m and M)
inline int clamp(int x, int m, int M)
{
    return min(M,max(m,x));
}
inline float clamp(float x, float m, float M)
{
    return min(M,max(m,x));
}


// Forward declarations
template <typename Scalar, int Dimension>  struct TVector;
template <typename Scalar, int Dimension>  struct TPoint;
template <typename Point, typename Vector> struct TRay;
template <typename Scalar, int Dimension> struct TBox;
class Accelerator;
class Material;
class Surface;
class Sphere;
class Quad;
class Mesh;
class WavefrontOBJ;
class Light;
class Scene;
class Color3f;

// Basic Dirt data structures (vectors, points, rays, bounding boxes,
// kd-trees) are oblivious to the underlying data type and dimension.
// The following list of typedefs establishes some convenient aliases
// for specific types.
typedef TVector<float, 1>       Vector1f;
typedef TVector<float, 2>       Vector2f;
typedef TVector<float, 3>       Vector3f;
typedef TVector<float, 4>       Vector4f;
typedef TVector<double, 1>      Vector1d;
typedef TVector<double, 2>      Vector2d;
typedef TVector<double, 3>      Vector3d;
typedef TVector<double, 4>      Vector4d;
typedef TVector<int, 1>         Vector1i;
typedef TVector<int, 2>         Vector2i;
typedef TVector<int, 3>         Vector3i;
typedef TVector<int, 4>         Vector4i;
typedef TPoint<float, 1>        Point1f;
typedef TPoint<float, 2>        Point2f;
typedef TPoint<float, 3>        Point3f;
typedef TPoint<float, 4>        Point4f;
typedef TPoint<double, 1>       Point1d;
typedef TPoint<double, 2>       Point2d;
typedef TPoint<double, 3>       Point3d;
typedef TPoint<double, 4>       Point4d;
typedef TPoint<int, 1>          Point1i;
typedef TPoint<int, 2>          Point2i;
typedef TPoint<int, 3>          Point3i;
typedef TPoint<int, 4>          Point4i;
typedef TRay<Point2f, Vector2f> Ray2f;
typedef TRay<Point3f, Vector3f> Ray3f;
typedef TBox<float, 3>          Box3f;

#ifdef _WIN32
const std::string message_ccs("");
const std::string message_cce("");
const std::string error_ccs("");
const std::string error_cce("");
const std::string warning_ccs("");
const std::string warning_cce("");
#else
const std::string message_ccs("\033[1;32m");
const std::string message_cce("\033[0m");
const std::string error_ccs("\033[1;31m");
const std::string error_cce("\033[0m");
const std::string warning_ccs("\033[1;33m");
const std::string warning_cce("\033[0m");
#endif


//! Error signaling unimplemented features
#define put_your_code_here(txt) \
    do { \
        static bool been_here = false; if (!been_here) { \
            been_here = true; \
            warning("\nwarning: %s() not (fully) implemented at %s:%d.\n    msg: %s\n", __FUNCTION__, __FILE__, __LINE__, txt); \
        } } while (0);

//! Prints a message (printf style) and flushes the output (useful during debugging)
template <typename ... Args>
inline void message(const char *msg, const Args &... args)
{
    string msg2 = message_ccs + string(msg) + message_cce;
    tfm::format(std::cout, msg2.c_str(), args ...);
    std::cout << std::flush;
}

//! Prints an error message (printf style), flushes the output (useful during debugging) and asserts
template <typename ... Args>
inline void error(const char *msg, const Args &... args)
{
    string msg2 = error_ccs + string(msg) + error_cce;
    tfm::format(std::cerr, msg2.c_str(), args ...);
    std::cerr << std::flush;
}

//! Prints an error message (printf style), flushes the output (useful during debugging) and asserts
template <typename ... Args>
inline void warning(const char *msg, const Args &... args)
{
    string msg2 = warning_ccs + string(msg) + warning_cce;
    tfm::format(std::cout, msg2.c_str(), args ...);
    std::cout << std::flush;
}

//! Checks for error, prints an error message (printf style), flushes the output (useful during debugging) and asserts
template <typename ... Args>
inline void error_if_not(bool check, const char* msg, const Args & ... args)
{
    if (check)
        return;
    error(msg, args ...);
}

//! Python-style range: iterates from min to max in range-based for loops
/*!
    To use:
        for(int i = 0; i < 100; i++) { ... }     // old way
        for(auto i : range(100))     { ... }     // new way
*/
struct range
{
    int min = 0;     // start
    int max = 1;     // end

    // iterator
    struct iterator
    {
        int current;

        iterator(int value) : current(value)
        {
        }

        int & operator*()
        {
            return current;
        }

        iterator & operator++ ()
        {
            ++current; return *this;
        }

        bool operator!=(const iterator &other)
        {
            return current != other.current;
        }
    };

    // constructor for range in [0,max)
    range(int max) : max(max)
    {
    }

    // constructor for range in [min,max)
    range(int min, int max) : min(min), max(max)
    {
    }

    // iterator support
    iterator begin()
    {
        return iterator(min);
    }
    // iterator support
    iterator end()
    {
        return iterator(max);
    }
};

//! Linear interpolation.
/*!
    Linearly interpolates between \a a and \a b, using parameter t.
    \param a A value.
    \param b Another value.
    \param t A blending factor of \a a and \a b.
    \return Linear interpolation of \a a and \b -
            a value between a and b if \a t is between 0 and 1.
 */
template <typename T, typename S>
inline T
lerp(T a, T b, S t)
{
    return T((S(1)-t) * a + t * b);
}

//! Inverse linear interpolation.
template <typename T>
inline T
lerpFactor(T a, T b, T m)
{
    return (m - a) / (b - a);
}


//! Barycentric interpolation.
template <typename T, typename S>
inline T
bary(T a, T b, T c, S s, S t)
{
    return (S(1)-s-t)*a + s*b + t*c;
}


//! Convert radians to degrees
inline float rad2deg(float value) { return value * (180.0f / M_PI); }

//! Convert degrees to radians
inline float deg2rad(float value) { return value * (M_PI / 180.0f); }


//! Convert a time value in milliseconds into a human-readable string
extern std::string timeString(double time, bool precise = false);

//! Convert a memory amount in bytes into a human-readable string
extern std::string memString(size_t size, bool precise = false);

//! Tokenize a string into a list by splitting at 'delim'
extern std::vector<std::string> tokenize(const std::string &s, const std::string &delim = ", ", bool includeEmpty = false);

//! Convert a string into an unsigned integer value
extern unsigned int toUInt(const std::string &str);

//! Simple exception class, which stores a human-readable error description
class DirtException: public std::runtime_error
{
public:
    //! Variadic template constructor to support printf-style arguments
    template <typename ... Args>
    DirtException(const char *fmt, const Args &... args)
        : std::runtime_error(tfm::format(fmt, args ...))
    {
    }
};

// Acceleration structure stats parameters
extern uint64_t primitives_intersected;
extern uint64_t rays_traced;

#define INCREMENT_INTERSECTED_PRIMS primitives_intersected++
#define INCREMENT_TRACED_RAYS rays_traced++
