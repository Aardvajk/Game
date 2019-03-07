#include "TextureVertex.h"

std::vector<Gx::VertexElement> TextureVertex::declaration()
{
    return { Gx::VertexElement::Position, Gx::VertexElement::Color, Gx::VertexElement::TexCoord };
}
