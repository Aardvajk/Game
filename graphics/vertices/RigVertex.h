#ifndef RIGVERTEX_H
#define RIGVERTEX_H

#include <GxMaths/GxVector.h>
#include <GxMaths/GxColor.h>

#include <GxGraphics/GxVertexDeclaration.h>

struct RigVertex
{
    Gx::Vec3 position;
    Gx::Vec3 normal;
    Gx::Rgba color;
    unsigned char indices[4];
    float weights[4];

    static std::vector<Gx::VertexElement> declaration();
};

#endif // RIGVERTEX_H
