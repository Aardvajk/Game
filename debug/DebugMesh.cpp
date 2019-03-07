#include "DebugMesh.h"

#include <vector>
#include <cmath>

#include <pcx/datastream.h>

namespace
{

inline pcx::data_ostream &operator<<(pcx::data_ostream &os, const Gx::Vec2 &v){ return os << v.x << v.y; }
inline pcx::data_ostream &operator<<(pcx::data_ostream &os, const Gx::Vec3 &v){ return os << v.x << v.y << v.z; }

class Face
{
public:
    Face() = default;
    Face(std::initializer_list<std::size_t> indices){ std::size_t i = 0; for(auto index: indices) e[i++] = index; }

    std::size_t &operator[](std::size_t index){ return e[index]; }
    std::size_t operator[](std::size_t index) const { return e[index]; }

private:
    std::size_t e[3];
};

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

pcx::buffer writeSmoothMesh(const std::vector<Gx::Vec3> &vs, const std::vector<Face> &fs, Gx::Color color)
{
    std::vector<std::vector<Gx::Vec3> > normals(vs.size());

    for(auto i: fs)
    {
        Gx::Vec3 a = vs[i[0]];
        Gx::Vec3 b = vs[i[1]];
        Gx::Vec3 c = vs[i[2]];

        Gx::Vec3 n = normal(a, b, c);

        normals[i[0]].push_back(n);
        normals[i[1]].push_back(n);
        normals[i[2]].push_back(n);
    }

    std::vector<Gx::Vec3> vn(vs.size());
    for(std::size_t i = 0; i < normals.size(); ++i) vn[i] = average(normals[i]);

    pcx::data_osstream ds;

    for(auto i: fs)
    {
        ds << vs[i[0]] << vn[i[0]] << Gx::Rgba(color) << Gx::Vec2(0, 0);
        ds << vs[i[1]] << vn[i[1]] << Gx::Rgba(color) << Gx::Vec2(0, 0);
        ds << vs[i[2]] << vn[i[2]] << Gx::Rgba(color) << Gx::Vec2(0, 0);
    }

    return ds.data();
}

pcx::buffer writeFlatMesh(const std::vector<Gx::Vec3> &vs, const std::vector<Face> &fs, Gx::Color color)
{
    pcx::data_osstream ds;

    for(std::size_t i = 0; i < fs.size(); ++i)
    {
        auto &e = fs[i];
        auto n = normal(vs[e[0]], vs[e[1]], vs[e[2]]);

        ds << vs[e[0]] << n << Gx::Rgba(color) << Gx::Vec2(0, 0);
        ds << vs[e[1]] << n << Gx::Rgba(color) << Gx::Vec2(0, 0);
        ds << vs[e[2]] << n << Gx::Rgba(color) << Gx::Vec2(0, 0);
    }

    return ds.data();
}

}

pcx::buffer DebugMesh::cuboidToBuffer(const Gx::Vec3 &dims, const Gx::Color &color)
{
    std::vector<Gx::Vec3> vs;

    Gx::Vec3 d = dims / 2;

    vs.push_back(Gx::Vec3(-d.x, d.y, -d.z));
    vs.push_back(Gx::Vec3(d.x, d.y, -d.z));
    vs.push_back(Gx::Vec3(d.x, -d.y, -d.z));
    vs.push_back(Gx::Vec3(-d.x, -d.y, -d.z));

    vs.push_back(Gx::Vec3(-d.x, d.y, d.z));
    vs.push_back(Gx::Vec3(d.x, d.y, d.z));
    vs.push_back(Gx::Vec3(d.x, -d.y, d.z));
    vs.push_back(Gx::Vec3(-d.x, -d.y, d.z));

    std::vector<Face> fs;

    fs.push_back({ 0, 1, 2 });
    fs.push_back({ 0, 2, 3 });
    fs.push_back({ 1, 5, 6 });
    fs.push_back({ 1, 6, 2 });
    fs.push_back({ 4, 0, 3 });
    fs.push_back({ 4, 3, 7 });
    fs.push_back({ 5, 4, 7 });
    fs.push_back({ 5, 7, 6 });
    fs.push_back({ 4, 5, 1 });
    fs.push_back({ 4, 1, 0 });
    fs.push_back({ 3, 2, 6 });
    fs.push_back({ 3, 6, 7 });

    return writeFlatMesh(vs, fs, color);
}

pcx::buffer DebugMesh::capsuleToBuffer(unsigned rings, unsigned segments, float radius, float height, const Gx::Color &color)
{
    std::vector<Gx::Vec3> vs;
    std::vector<Face> fs;

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
            vs.push_back(Gx::Vec3(0, radius + ht, 0));
        }
        else if(ring == rings - 1)
        {
            vs.push_back(Gx::Vec3(0, -(radius + ht), 0));
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

                vs.push_back(Gx::Vec3(-std::sin(a) * xdelta, ydelta + h, std::cos(a) * xdelta));
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
                fs.push_back(Face({ start, start + 1 + j, start + 1 + i }));
            }
        }
        else if(ring == rings - 2)
        {
            unsigned base = vs.size() - 1;

            for(unsigned i = 0; i < segments; ++i)
            {
                unsigned j = (i < segments - 1 ? i + 1 : 0);
                fs.push_back(Face({ start + base - segments + i, start + base - segments + j, start + base }));
            }
        }
        else
        {
            unsigned vertex = 1 + ((ring - 1) * segments);

            for(unsigned i = 0; i < segments; ++i)
            {
                unsigned j = (i < segments - 1 ? i + 1 : 0);

                fs.push_back(Face({ start + vertex + i, start + vertex + j, start + vertex + j + segments }));
                fs.push_back(Face({ start + vertex + i, start + vertex + j + segments, start + vertex + i + segments }));
            }
        }
    }

    return writeSmoothMesh(vs, fs, color);
}
