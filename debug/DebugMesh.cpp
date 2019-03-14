#include "DebugMesh.h"

#include <GxMaths/GxColor.h>

#include <vector>
#include <cmath>

#include <pcx/datastream.h>

namespace
{

inline pcx::data_ostream &operator<<(pcx::data_ostream &os, const Gx::Vec2 &v){ return os << v.x << v.y; }
inline pcx::data_ostream &operator<<(pcx::data_ostream &os, const Gx::Vec3 &v){ return os << v.x << v.y << v.z; }

Gx::Vec3 normal(const Gx::Vec3 &v0, const Gx::Vec3 &v1, const Gx::Vec3 &v2)
{
    Gx::Vec3 a = v0;
    Gx::Vec3 b = v1 - a;
    Gx::Vec3 c = v2 - a;

    return b.cross(c).normalized();
}

Gx::Vec3 average(const std::vector<Gx::Vec3> &values)
{
    Gx::Vec3 v(0, 0, 0);
    for(const auto &i: values) v += i;

    if(!values.empty())
    {
        v /= float(values.size());
    }

    return v;
}

}

DebugMesh::DebugMesh()
{
}

DebugMesh::DebugMesh(const std::vector<Gx::Vec3> &vs, const std::vector<Gx::PolyhedronShape::Face> &fs) : vs(vs), fs(fs)
{
}

DebugMesh DebugMesh::cuboid(const Gx::Vec3 &dims)
{
    DebugMesh m;
    Gx::Vec3 d = dims / 2;

    m.vs.push_back(Gx::Vec3(-d.x, d.y, -d.z));
    m.vs.push_back(Gx::Vec3(d.x, d.y, -d.z));
    m.vs.push_back(Gx::Vec3(d.x, -d.y, -d.z));
    m.vs.push_back(Gx::Vec3(-d.x, -d.y, -d.z));

    m.vs.push_back(Gx::Vec3(-d.x, d.y, d.z));
    m.vs.push_back(Gx::Vec3(d.x, d.y, d.z));
    m.vs.push_back(Gx::Vec3(d.x, -d.y, d.z));
    m.vs.push_back(Gx::Vec3(-d.x, -d.y, d.z));

    m.fs.push_back({ 0, 1, 2, 3 });
    m.fs.push_back({ 1, 5, 6, 2 });
    m.fs.push_back({ 4, 0, 3, 7 });
    m.fs.push_back({ 5, 4, 7, 6 });
    m.fs.push_back({ 4, 5, 1, 0 });
    m.fs.push_back({ 3, 2, 6, 7 });

    return m;
}

DebugMesh DebugMesh::capsule(unsigned rings, unsigned segments, float radius, float height)
{
    DebugMesh m;

    const float twoPi = M_PI * 2;
    const float ydiff = 1.0f / (rings - 1.0f);
    const float xdiff = twoPi / segments;

    float ht = (height - (radius * 2)) / 2;

    float y = 0;

    unsigned start = 0;

    for(unsigned ring = 0; ring < rings; ++ring)
    {
        if(ring == 0)
        {
            m.vs.push_back(Gx::Vec3(0, radius + ht, 0));
        }
        else if(ring == rings - 1)
        {
            m.vs.push_back(Gx::Vec3(0, -(radius + ht), 0));
        }
        else
        {
            float xdelta = std::sin(y * M_PI) * radius;
            float ydelta = std::cos(y * M_PI) * radius;

            float a = 0;
            for(size_t x = 0; x < segments; ++x)
            {
                float h = ht;
                if(ydelta < 0) h = -ht;

                m.vs.push_back(Gx::Vec3(-std::sin(a) * xdelta, ydelta + h, std::cos(a) * xdelta));
                a += xdiff;
            }
        }

        y += ydiff;
    }

    for(unsigned ring = 0; ring < rings - 1; ++ring)
    {
        if(ring == 0)
        {
            for(unsigned i = 0; i < segments; ++i)
            {
                unsigned j = (i < segments - 1 ? i + 1 : 0);
                m.fs.push_back({ start, start + 1 + j, start + 1 + i });
            }
        }
        else if(ring == rings - 2)
        {
            unsigned base = m.vs.size() - 1;

            for(unsigned i = 0; i < segments; ++i)
            {
                unsigned j = (i < segments - 1 ? i + 1 : 0);
                m.fs.push_back({ start + base - segments + i, start + base - segments + j, start + base });
            }
        }
        else
        {
            unsigned vertex = 1 + ((ring - 1) * segments);

            for(unsigned i = 0; i < segments; ++i)
            {
                unsigned j = (i < segments - 1 ? i + 1 : 0);

                m.fs.push_back({ start + vertex + i, start + vertex + j, start + vertex + j + segments, start + vertex + i + segments });
            }
        }
    }

    return m;
}

DebugMesh DebugMesh::sphere(unsigned rings, unsigned segments, float radius)
{
    return capsule(rings, segments, radius, radius * 2);
}

DebugMesh DebugMesh::tetrahedron(float radius)
{
    DebugMesh m;

    m.vs = { { radius, radius, radius }, { -radius,  radius, -radius }, { radius, -radius, -radius }, { -radius, -radius, radius } };
    m.fs = { { 1, 2, 3 }, { 0, 3, 2 }, { 0, 1, 3 }, { 0, 2, 1 } };

    return m;
}

DebugMesh DebugMesh::cone(unsigned segments, float radius, float height)
{
    DebugMesh m;

    const float twoPi = M_PI * 2;
    const float halfHt = height / 2;
    const float inc = twoPi / segments;

    m.vs.push_back({ 0, halfHt, 0 });

    for(float a = 0; a < twoPi; a += inc)
    {
        m.vs.push_back({ std::sin(a) * radius, -halfHt, std::cos(a) * radius });
    }

    for(std::size_t i = 1; i < m.vs.size(); ++i)
    {
        std::size_t j = i < m.vs.size() - 1 ? i + 1 : 1;

        m.fs.push_back({ 0, i, j });
    }

    for(std::size_t i = 2; i < m.vs.size(); ++i)
    {
        std::size_t j = i < m.vs.size() - 1 ? i + 1 : 2;

        m.fs.push_back({ 1, j, i });
    }

    return m;
}

pcx::buffer DebugMesh::flatMesh(const DebugMesh &m, const Gx::Color &color)
{
    pcx::data_osstream ds;

    for(std::size_t i = 0; i < m.fs.size(); ++i)
    {
        auto &e = m.fs[i];
        auto n = normal(m.vs[e[0]], m.vs[e[1]], m.vs[e[2]]);

        for(std::size_t j = 1; j < e.size() - 1; ++j)
        {
            ds << m.vs[e[0]] << n << Gx::Rgba(color) << Gx::Vec2(0, 0);
            ds << m.vs[e[j]] << n << Gx::Rgba(color) << Gx::Vec2(0, 0);
            ds << m.vs[e[j + 1]] << n << Gx::Rgba(color) << Gx::Vec2(0, 0);
        }
    }

    return ds.data();
}

pcx::buffer DebugMesh::smoothMesh(const DebugMesh &m, const Gx::Color &color)
{
    std::vector<std::vector<Gx::Vec3> > normals(m.vs.size());

    for(auto i: m.fs)
    {
        Gx::Vec3 a = m.vs[i[0]];
        Gx::Vec3 b = m.vs[i[1]];
        Gx::Vec3 c = m.vs[i[2]];

        Gx::Vec3 n = normal(a, b, c);

        for(std::size_t j = 1; j < i.size() - 1; ++j)
        {
            normals[i[0]].push_back(n);
            normals[i[j]].push_back(n);
            normals[i[j + 1]].push_back(n);
        }
    }

    std::vector<Gx::Vec3> vn(m.vs.size());
    for(std::size_t i = 0; i < normals.size(); ++i) vn[i] = average(normals[i]);

    pcx::data_osstream ds;

    for(auto i: m.fs)
    {
        for(std::size_t j = 1; j < i.size() - 1; ++j)
        {
            ds << m.vs[i[0]] << vn[i[0]] << Gx::Rgba(color) << Gx::Vec2(0, 0);
            ds << m.vs[i[j]] << vn[i[j]] << Gx::Rgba(color) << Gx::Vec2(0, 0);
            ds << m.vs[i[j + 1]] << vn[i[j + 1]] << Gx::Rgba(color) << Gx::Vec2(0, 0);
        }
    }

    return ds.data();
}
