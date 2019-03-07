#include "MeshVertex.h"

std::vector<Gx::VertexElement> MeshVertex::declaration()
{
    return { Gx::VertexElement::Position, Gx::VertexElement::Normal, Gx::VertexElement::Color, Gx::VertexElement::TexCoord };
}
