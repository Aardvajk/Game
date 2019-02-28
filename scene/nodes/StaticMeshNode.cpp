#include "StaticMeshNode.h"

#include "graphics/Graphics.h"
#include "graphics/VertexBuffer.h"

#include "graphics/vertices/MeshVertex.h"

StaticMeshNode::StaticMeshNode(const VertexBuffer *mesh, const Gx::Matrix &transform) : mesh(mesh), tr(transform)
{
}

void StaticMeshNode::render(Graphics &graphics) const
{
    graphics.meshShader->setMatrix(graphics.device, "world", tr);
    mesh->renderTriangleList(graphics.device, sizeof(MeshVertex));
}
