#include "PcNode.h"

#include "graphics/Graphics.h"
#include "graphics/VertexBuffer.h"

#include "graphics/vertices/MeshVertex.h"

#include <GxGraphics/GxShader.h>

PcNode::PcNode(const VertexBuffer *mesh, const Gx::Matrix &transform) : mesh(mesh), tr(transform)
{
}

bool PcNode::pass(RenderPass type) const
{
    return type != RenderPass::EnvironmentDepth;
}

RenderType PcNode::type() const
{
    return RenderType::Object;
}

RenderKey PcNode::key() const
{
    return { };
}

void PcNode::render(RenderPass pass, Graphics &graphics, SceneParams &params) const
{
    graphics.currentVertexShader()->setMatrix(graphics.device, "world", tr);
    mesh->renderTriangleList(graphics.device, sizeof(MeshVertex));
}

void PcNode::updateTransform(const Gx::Matrix &transform)
{
    tr = transform;
}

