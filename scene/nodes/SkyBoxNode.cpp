#include "SkyBoxNode.h"

#include "graphics/Graphics.h"
#include "graphics/VertexBuffer.h"

#include "graphics/vertices/PositionVertex.h"

SkyBoxNode::SkyBoxNode(const VertexBuffer *mesh, const Gx::CubeMap *diffuse) : mesh(mesh), diffuse(diffuse)
{
}

bool SkyBoxNode::pass(RenderPass type) const
{
    return type == RenderPass::Normal;
}

RenderType SkyBoxNode::type() const
{
    return RenderType::SkyBox;
}

RenderKey SkyBoxNode::key() const
{
    return { };
}

void SkyBoxNode::render(RenderPass pass, Graphics &graphics, SceneParams &params) const
{
    graphics.device.setTexture(0, *diffuse);
    mesh->renderTriangleList(graphics.device, sizeof(PositionVertex));
}
