#include "ColorVertex.h"

std::vector<Gx::VertexElement> ColorVertex::declaration()
{
    return { Gx::VertexElement::Position, Gx::VertexElement::Color };
}
