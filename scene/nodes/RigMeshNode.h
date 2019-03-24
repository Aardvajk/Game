#ifndef RIGMESHNODE_H
#define RIGMESHNODE_H

#include "scene/nodes/SceneNode.h"

#include <GxMaths/GxMatrix.h>

class VertexBuffer;

class RigMeshNode : public SceneNode
{
public:
    RigMeshNode(const VertexBuffer *mesh, const Gx::Matrix &transform);

    virtual bool pass(RenderPass type) const override;
    virtual RenderType type() const override;
    virtual RenderKey key() const override;

    virtual void render(RenderPass pass, Graphics &graphics, SceneParams &params) const override;

    void updateTransform(const Gx::Matrix &value);

private:
    const VertexBuffer *mesh;
    Gx::Matrix tr;
};

#endif // RIGMESHNODE_H
