#ifndef PCNODE_H
#define PCNODE_H

#include "scene/nodes/SceneNode.h"

#include <GxMaths/GxMatrix.h>

class VertexBuffer;

class PcNode : public SceneNode
{
public:
    PcNode(const VertexBuffer *mesh, const Gx::Matrix &transform);

    virtual bool pass(RenderPass type) const override;
    virtual RenderType type() const override;
    virtual RenderKey key() const override;

    virtual void render(RenderPass pass, Graphics &graphics, SceneParams &params) const override;

    void updateTransform(const Gx::Matrix &transform);

private:
    const VertexBuffer *mesh;
    Gx::Matrix tr;
};

#endif // PCODE_H
