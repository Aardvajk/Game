#include "StaticMeshNode.h"

#include "graphics/Graphics.h"
#include "graphics/VertexBuffer.h"

#include "graphics/vertices/MeshVertex.h"

#include <GxGraphics/GxShader.h>

StaticMeshNode::StaticMeshNode(const VertexBuffer *mesh, const Gx::Matrix &transform) : mesh(mesh), tr(transform)
{
}

bool StaticMeshNode::pass(RenderPass type) const
{
    return type == RenderPass::Normal;
}

RenderType StaticMeshNode::type() const
{
    return RenderType::Mesh;
}

void StaticMeshNode::render(RenderPass pass, Graphics &graphics, const SceneParams &params) const
{
    graphics.meshShader->setMatrix(graphics.device, "world", tr);
    mesh->renderTriangleList(graphics.device, sizeof(MeshVertex));
}

void StaticMeshNode::updateTransform(const Gx::Matrix &transform)
{
    tr = transform;
}

