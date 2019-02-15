#ifndef MODEL_H
#define MODEL_H

#include "graphics/Graphics.h"

#include <GxGraphics/GxVertexBuffer.h>

#include <pcx/non_copyable.h>

#include <vector>

class Model : public pcx::non_copyable
{
public:
    Model();

    Graphics::Ptr<Gx::VertexBuffer> addVertexBuffer(Graphics::Handle<Gx::VertexBuffer> &&v);

private:
    std::vector<Graphics::Handle<Gx::VertexBuffer> > vertexBuffers;
};

#endif // MODEL_H
