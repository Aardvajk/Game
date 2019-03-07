#ifndef TEXTUREVERTEX_H
#define TEXTUREVERTEX_H

#include <GxMaths/GxVector.h>
#include <GxMaths/GxColor.h>

#include <GxGraphics/GxVertexDeclaration.h>

struct TextureVertex
{
    Gx::Vec3 position;
    Gx::Rgba color;
    Gx::Vec2 texCoords;

    static std::vector<Gx::VertexElement> declaration();
};

#endif // TEXTUREVERTEX_H
