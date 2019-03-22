#ifndef POSITIONVERTEX_H
#define POSITIONVERTEX_H

#include <GxMaths/GxVector.h>
#include <GxMaths/GxColor.h>

#include <GxGraphics/GxVertexDeclaration.h>

struct PositionVertex
{
    Gx::Vec3 position;

    static std::vector<Gx::VertexElement> declaration();
};

#endif // POSITIONVERTEX_H
