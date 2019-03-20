#include "StaticMeshNode.h"

#include "graphics/Graphics.h"
#include "graphics/VertexBuffer.h"

#include "graphics/vertices/MeshVertex.h"

#include <GxGraphics/GxShader.h>

StaticMeshNode::StaticMeshNode(const VertexBuffer *mesh, const RenderKey &key, const Gx::Matrix &transform) : mesh(mesh), rk(key), tr(transform)
{
}

bool StaticMeshNode::pass(RenderPass type) const
{
    return type == RenderPass::Normal || type == RenderPass::EnvironmentDepth;
}

RenderType StaticMeshNode::type() const
{
    return RenderType::Environment;
}

RenderKey StaticMeshNode::key() const
{
    return rk;
}

void StaticMeshNode::render(RenderPass pass, Graphics &graphics, SceneParams &params) const
{
    graphics.currentVertexShader()->setMatrix(graphics.device, "world", tr);
    mesh->renderTriangleList(graphics.device, sizeof(MeshVertex));
}

void StaticMeshNode::updateTransform(const Gx::Matrix &value)
{
    tr = value;
}
