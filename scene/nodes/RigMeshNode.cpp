#include "RigMeshNode.h"

#include "graphics/Graphics.h"
#include "graphics/VertexBuffer.h"

#include "graphics/vertices/RigVertex.h"

#include <GxGraphics/GxShader.h>

RigMeshNode::RigMeshNode(const VertexBuffer *mesh, const Gx::Matrix &transform) : mesh(mesh), tr(transform)
{
}

bool RigMeshNode::pass(RenderPass type) const
{
    return type == RenderPass::Normal || type == RenderPass::ObjectDepth;
}

RenderType RigMeshNode::type() const
{
    return RenderType::Rig;
}

RenderKey RigMeshNode::key() const
{
    return { };
}

void RigMeshNode::render(RenderPass pass, Graphics &graphics, SceneParams &params) const
{
    std::vector<Gx::Matrix> bv;
    for(int i = 0; i < 33; ++i) bv.push_back(Gx::Matrix::identity());

    graphics.currentVertexShader()->setMatrix(graphics.device, "world", tr);
    graphics.currentVertexShader()->setMatrixArray(graphics.device, "bones", bv);

    mesh->renderTriangleList(graphics.device, sizeof(RigVertex));
}

void RigMeshNode::updateTransform(const Gx::Matrix &value)
{
    tr = value;
}
