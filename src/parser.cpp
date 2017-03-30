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

#include "parser.h"
#include <Eigen/Geometry>

// local functions
namespace
{

// print usage information
void cmdlinePrintUsage(const CommandLine & cmd)
{
    auto usage = "usage: " + cmd.progname;
    for (auto opt : cmd.options)
    {
        usage += " ";
        auto optname = (opt.flag == "") ? "--"+opt.name : "-"+opt.flag;
        auto optval = (opt.type != typeid(bool)) ? " <"+opt.name+">" : "";
        if (opt.opt)
            usage += "[" + optname + optval + "]";
        else
            usage += optname + optval;
    }
    for (auto arg : cmd.arguments)
    {
        usage += " ";
        if (arg.opt)
            usage += "[" + arg.name + "]";
        else
            usage += arg.name;
    }
    usage += "\n\n";
    if (not cmd.options.empty() or not cmd.arguments.empty())
    {
        usage += "options:\n";
        for (auto opt : cmd.options)
        {
            usage += "    " + ((opt.flag == "") ? "" : "-"+opt.flag+"/") +
                     "--" + opt.name + ((opt.type==typeid(bool)) ? "" : " "+opt.name) + "\n";
            usage += "        " + opt.desc + "\n";
        }
        for (auto arg : cmd.arguments)
        {
            usage += "    " + arg.name + ((arg.type==typeid(bool)) ? "" : " "+arg.name) + "\n";
            usage += "        " + arg.desc + "\n";
        }
        usage += "\n\n";
    }
    message(usage.c_str());
}

// parse error
void cmdlineParseError(const string & msg, const CommandLine & cmd)
{
    cmdlinePrintUsage(cmd);
    message("%s\n\n",msg.c_str());
    exit(EXIT_FAILURE);
}


// parse value
json cmdlineParseValue(const string & str, const CommandLine::Argument & opt, const CommandLine & cmd)
{
    try
    {
        if (typeid(int) == opt.type)
        {
            return json(std::stoi(str));
        }
        else if (typeid(float) == opt.type)
        {
            return json(std::stod(str));
        }
        else if (typeid(double) == opt.type)
        {
            return json(std::stod(str));
        }
        else if (typeid(string) == opt.type)
        {
            return json(str);
        }
        else
            error("unknown type");
    }
    catch (const std::exception &)
    {
        cmdlineParseError(tfm::format("invalid value for %s", opt.name.c_str()), cmd);
    }
    return json();
}

Eigen::Matrix4f parseSingleTransform(const json & j)
{
    Eigen::Matrix4f m = Eigen::Matrix4f::Identity();
    if (j.count("from") || j.count("at") || j.count("up"))
    {
        Vector3f from = Vector3f::UnitZ(), at = Vector3f::Zero(), up = Vector3f::UnitY();
        Parser::get(j, from, "from");
        Parser::get(j, at, "at");
        Parser::get(j, up, "up");

        Vector3f dir = (from - at).normalized();
        Vector3f left = up.normalized().cross(dir).normalized();
        Vector3f newUp = dir.cross(left).normalized();

        m.topLeftCorner<3,4>() << left, newUp, dir, from;
        return m;
    }
    else if (j.count("o") || j.count("x") || j.count("y") || j.count("z"))
    {
        Vector3f o(0,0,0), x(1, 0, 0), y(0, 1, 0), z(0, 0, 1);
        Parser::get(j, o, "o");
        Parser::get(j, x, "x");
        Parser::get(j, y, "y");
        Parser::get(j, z, "z");
        m.topLeftCorner<3,4>() << x, y, z, o;
        return m;
    }
    else if (j.count("translate"))
    {
        Eigen::Vector3f v(0,0,0);
        Parser::get(j["translate"], v);
        m.topRightCorner<3,1>() << v;
        return m;
    }
    else if (j.count("scale"))
    {
        Vector3f v(1,1,1);
        if (j.size() == 1)
        {
            float s = 1.0f;
            Parser::get(j["scale"], s);
            v = Vector3f(s,s,s);
        }
        else if (j.size() == 3)
            Parser::get(j["scale"], v);

        m.diagonal().head<3>() << v;
        return m;
    }
    else if (j.count("axis") || j.count("angle"))
    {
        float angle = 0.0;
        Vector3f axis(1,0,0);
        Parser::get(j, axis, "axis");
        Parser::get(j, angle, "angle");
        m.topLeftCorner<3,3>() << Eigen::AngleAxis<float>(deg2rad(angle), axis).matrix();
        return m;
    }
    else if (j.count("matrix"))
    {
        Parser::get(j["matrix"], m);
        return m;
    }
    else
        error("unrecognized \"transform\" command: %s.\n", j.dump());

    return m;
}

} // namespace


void Parser::get(const json & j, Transform & v)
{
    Eigen::Matrix4f m = v.getMatrix();
    if (j.is_array())
    {

        // multiple transformation commands listed in order
        for (auto & element : j)
        {
            m = parseSingleTransform(element) * m;
        }
    }
    else if (j.is_object())
    {
        // a single transformation
        m = parseSingleTransform(j);
    }
    else
        error("\"transform\" command must be either an array or an object.");

    v = Transform(m);
}


// json handling
json loadJSON(const string & filename)
{
    // open file
    std::ifstream stream(filename.c_str(), std::ifstream::in);
    error_if_not(stream.good(), "cannot open file: %s\n", filename.c_str());
    json j;
    stream >> j;
    stream.close();
    return j;
}


// parsing values
json parseCmdline(const vector<string> & args, const CommandLine & cmd)
{
    auto parsed = json::object_t();
    auto largs = args; // make a copy to change
    set<string> visited;
    for (auto opt : cmd.options)
    {
        auto pos = -1;
        for (auto i : range(int(largs.size())))
        {
            if (largs[i] == "--"+opt.name or largs[i] == "-"+opt.flag)
            {
                pos = i; break;
            }
        }
        if (pos < 0)
        {
            if (opt.opt)
                parsed[opt.name] = opt.def;
            else
                cmdlineParseError(tfm::format("required option -%s", opt.flag.c_str()), cmd);
        }
        else
        {
            if (typeid(bool) != opt.type)
            {
                if (pos == int(largs.size())-1)
                    cmdlineParseError(tfm::format("no value for argument %s", opt.name.c_str()), cmd);
                auto sval = largs[pos+1];
                largs.erase(largs.begin()+pos,largs.begin()+pos+2);
                parsed[opt.name] = cmdlineParseValue(sval, opt, cmd);
            }
            else
            {
                parsed[opt.name] = true;
                largs.erase(largs.begin()+pos);
            }
        }
    }
    for (auto arg : largs)
    {
        if (arg[0] == '-')
            cmdlineParseError(tfm::format("unknown option %s", arg.c_str()), cmd);
    }
    for (auto arg : cmd.arguments)
    {
        if (largs.empty())
        {
            if (arg.opt)
                parsed[arg.name] = arg.def;
            else
                cmdlineParseError(tfm::format("missing required argument %s", arg.name.c_str()), cmd);
        }
        else
        {
            parsed[arg.name] = cmdlineParseValue(largs[0], arg, cmd);
            largs.erase(largs.begin());
        }
    }
    if (not largs.empty())
        cmdlineParseError("too many arguments",cmd);
    return json(parsed);
}

// parsing values
json parseCmdline(int argc, char** argv, const CommandLine & cmd)
{
    auto args = vector<string>();
    for (auto i : range(1, argc))
        args.push_back(argv[i]);
    return parseCmdline(args, cmd);
}
