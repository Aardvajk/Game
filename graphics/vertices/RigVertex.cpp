#include "RigVertex.h"

std::vector<Gx::VertexElement> RigVertex::declaration()
{
    return { Gx::VertexElement::Position, Gx::VertexElement::Normal, Gx::VertexElement::Color, Gx::VertexElement::Indices, Gx::VertexElement::Weights };
}
