#include "Model.h"

Model::Model()
{
}

Graphics::Ptr<Gx::VertexBuffer> Model::addVertexBuffer(Graphics::Handle<Gx::VertexBuffer> &&v)
{
    vertexBuffers.push_back(std::move(v));
    return vertexBuffers.back().ptr();
}
