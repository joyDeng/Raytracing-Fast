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

#include "common.h"
#include "vector.h"
#include "color.h"
#include "transform.h"

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable : 4706)
#endif
#include <json.hpp>
#if defined(_MSC_VER)
#pragma warning(pop) 
#endif

using json = nlohmann::json;

// json loading
json loadJSON(const string & filename);

//! Helper class to parse various parameter values from a JSON object
class Parser
{
public:
    template <typename T>
    static void values(const json & j, T* value, size_t n)
    {
        error_if_not(n == j.size(), "incorrect array size");
        for (auto i : range(int(n)))
            value[i] = j[i].get<T>();
    }

    // parse a basic type
    static void get(const json & j, bool & v)     {v = j.get<bool>();}
    static void get(const json & j, int & v)      {v = j.get<int>();}
    static void get(const json & j, float & v)    {v = j.get<float>();}
    static void get(const json & j, string & v)   {v = j.get<string>();}

    //! parse a vector of basic types
    template <typename T>
    static void get(const json & j, vector<T> & v)
    {
        v = j.get< vector<T> >();
    }

    //! parse an Eigen matrix or vector
    template <typename _Scalar, int _Rows, int _Cols>
    static void get(const json & j, Eigen::Matrix<_Scalar, _Rows, _Cols> & v)
    {
        values(j, v.data(), v.size());
    }

    //! parse an Eigen array
    template <typename _Scalar, int _Rows, int _Cols>
    static void get(const json & j, Eigen::Array<_Scalar, _Rows, _Cols> & v)
    {
        values(j, v.data(), v.size());
    }

    //! parse a transform
    static void get(const json & j, Transform & v);

    //! (optionally) parse a value
    template<typename T>
    static void get(const json & j, T & v, const string & name)
    {
        if (!j.count(name))
            return;
        get(j[name], v);
    }
};

// command line specification
struct CommandLine
{
    // description of command line argument
    struct Argument
    {
        string name;                    // complete name
        string flag;                    // flag string name
        string desc;                    // description
        const std::type_info &   type;  // supported: int, double, bool (flag), string
        bool opt;                       // whether it is optional argument
        json def;                       // default value
    };

    // parameters
    string progname;                // program name
    string progdesc;                // program description
    vector<Argument>    options;    // program options (flags)
    vector<Argument>    arguments;  // program arguments

    CommandLine(const string &progname, const string &progdesc,
                const vector<Argument>&options, const vector<Argument>&arguments) :
        progname(progname), progdesc(progdesc), options(options), arguments(arguments)
    {
    }
};

// parsing command line arguments to json
json parseCmdline(const vector<string> & args, const CommandLine & cmd);
json parseCmdline(int argc, char** argv, const CommandLine & cmd);
