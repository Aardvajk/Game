#ifndef DEBUGMESH_H
#define DEBUGMESH_H

#include <GxMaths/GxVector.h>

#include <GxPhysics/GxShapes/GxPolyhedronShape.h>

#include <pcx/buffer.h>

#include <vector>

namespace Gx
{

class Color;

}

class DebugMesh
{
public:
    enum class FaceType
    {
        Flat,
        Smooth
    };

    DebugMesh();
    DebugMesh(const std::vector<Gx::Vec3> &vs, const std::vector<Gx::PolyhedronShape::Face> &fs);

    static DebugMesh cuboid(const Gx::Vec3 &dims);
    static DebugMesh capsule(unsigned rings, unsigned segments, float radius, float height);
    static DebugMesh sphere(unsigned rings, unsigned segments, float radius);
    static DebugMesh tetrahedron(float radius);
    static DebugMesh cone(unsigned segments, float radius, float height);
    static DebugMesh cylinder(unsigned segments, float radius, float height);

    static pcx::buffer flatMesh(const DebugMesh &m, const Gx::Color &color);
    static pcx::buffer smoothMesh(const DebugMesh &m, const Gx::Color &color);
    static pcx::buffer mixedMesh(const DebugMesh &m, const Gx::Color &color);

    std::vector<Gx::Vec3> vs;
    std::vector<Gx::PolyhedronShape::Face> fs;
    std::vector<FaceType> ft;
};

#endif // DEBUGMESH_H
