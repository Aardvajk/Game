#ifndef MESHVERTEX_H
#define MESHVERTEX_H

#include <GxMaths/GxVector.h>
#include <GxMaths/GxColor.h>

#include <GxGraphics/GxVertexDeclaration.h>

struct MeshVertex
{
    Gx::Vec3 position;
    Gx::Vec3 normal;
    Gx::Rgba color;

    static std::vector<Gx::VertexElement> declaration();
};

#endif // MESHVERTEX_H
