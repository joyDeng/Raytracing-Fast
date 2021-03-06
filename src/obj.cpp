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
/*
    This file is part of Nori, a simple educational ray tracer

    Copyright (c) 2015 by Wenzel Jakob

    Nori is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License Version 3
    as published by the Free Software Foundation.

    Nori is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "obj.h"
#include "timer.h"
#include <unordered_map>
#include <fstream>

namespace
{

//! Vertex indices used by the OBJ format
struct OBJVertex
{
    uint32_t p = (uint32_t) -1;
    uint32_t n = (uint32_t) -1;
    uint32_t uv = (uint32_t) -1;

    inline OBJVertex()
    {
    }

    inline OBJVertex(const std::string & string)
    {
        std::vector<std::string> tokens = tokenize(string, "/", true);

        if (tokens.size() < 1 || tokens.size() > 3)
            throw DirtException("Invalid vertex data: \"%s\"", string);

        p = toUInt(tokens[0]);

        if (tokens.size() >= 2 && !tokens[1].empty())
            uv = toUInt(tokens[1]);

        if (tokens.size() >= 3 && !tokens[2].empty())
            n = toUInt(tokens[2]);
    }

    inline bool operator==(const OBJVertex & v) const
    {
        return v.p == p && v.n == n && v.uv == uv;
    }
};

//! Hash function for OBJVertex
struct OBJVertexHash : std::unary_function<OBJVertex, size_t>
{
    std::size_t operator()(const OBJVertex & v) const
    {
        size_t hash = std::hash<uint32_t>()(v.p);
        hash = hash * 37 + std::hash<uint32_t>()(v.uv);
        hash = hash * 37 + std::hash<uint32_t>()(v.n);
        return hash;
    }
};

} // namespace


WavefrontOBJ::WavefrontOBJ(const Scene & scene, const json & j) : Mesh(scene, j)
{
    typedef std::unordered_map<OBJVertex, uint32_t, OBJVertexHash> VertexMap;

    std::string filename = j["filename"];

    std::ifstream is(filename);
    if (is.fail())
        throw DirtException("Unable to open OBJ file \"%s\"!", filename);

    cout << "Loading \"" << filename << "\" .. ";
    cout.flush();
    Timer timer;

    std::vector<Point3f>    positions;
    std::vector<Point2f>    texcoords;
    std::vector<Normal3f>   normals;
    std::vector<uint32_t>   indices;
    std::vector<OBJVertex>  vertices;
    VertexMap vertexMap;

    std::string line_str;
    while (std::getline(is, line_str))
    {
        std::istringstream line(line_str);

        std::string prefix;
        line >> prefix;

        if (prefix == "v")
        {
            Point3f p;
            line >> p.x() >> p.y() >> p.z();
            p = m_xform * p;
            positions.push_back(p);
        }
        else if (prefix == "vt")
        {
            Point2f tc;
            line >> tc.x() >> tc.y();
            texcoords.push_back(tc);
        }
        else if (prefix == "vn")
        {
            Normal3f n;
            line >> n.x() >> n.y() >> n.z();
            normals.push_back((m_xform * n).normalized());
        }
        else if (prefix == "f")
        {
            std::string v1, v2, v3, v4;
            line >> v1 >> v2 >> v3 >> v4;
            OBJVertex verts[6];
            int nVertices = 3;

            verts[0] = OBJVertex(v1);
            verts[1] = OBJVertex(v2);
            verts[2] = OBJVertex(v3);

            if (!v4.empty())
            {
                // This is a quad, split into two triangles
                verts[3] = OBJVertex(v4);
                verts[4] = verts[0];
                verts[5] = verts[2];
                nVertices = 6;
            }

            // Convert to an indexed vertex list
            for (auto i : range(nVertices))
            {
                const OBJVertex & v = verts[i];
                VertexMap::const_iterator it = vertexMap.find(v);
                if (it == vertexMap.end())
                {
                    vertexMap[v] = (uint32_t) vertices.size();
                    indices.push_back((uint32_t) vertices.size());
                    vertices.push_back(v);
                }
                else
                {
                    indices.push_back(it->second);
                }
            }
        }
    }

    m_F.resize(indices.size()/3);
    for (auto i : range(int(m_F.size())))
        m_F[i] = Vector3i(indices[3*i], indices[3*i+1], indices[3*i+2]);

    m_V.resize(vertices.size());
    for (auto i : range(int(vertices.size())))
        m_V[i] = positions.at(vertices[i].p-1);

    if (!normals.empty())
    {
        m_N.resize(vertices.size());
        for (auto i : range(int(vertices.size())))
            m_N[i] = normals.at(vertices[i].n-1);
    }

    if (!texcoords.empty())
    {
        m_UV.resize(vertices.size());
        for (auto i : range(int(vertices.size())))
            m_UV[i] = texcoords.at(vertices[i].uv-1);
    }

    cout << "done. (V=" << m_V.size() << ", F=" << m_F.size() << ", took "
         << timer.elapsedString() << " and "
         << memString(m_F.size() * sizeof(uint32_t) +
                 sizeof(float) * (m_V.size() + m_N.size() + m_UV.size()))
         << ")" << endl;
}
