#ifndef STATICMESHNODE_H
#define STATICMESHNODE_H

#include "scene/nodes/SceneNode.h"

#include <GxMaths/GxMatrix.h>

class VertexBuffer;

class StaticMeshNode : public SceneNode
{
public:
    StaticMeshNode(const VertexBuffer *mesh, const RenderKey &key, const Gx::Matrix &transform);

    virtual bool pass(RenderPass type) const override;
    virtual RenderType type() const override;
    virtual RenderKey key() const override;

    virtual void render(RenderPass pass, Graphics &graphics, SceneParams &params) const override;

    void updateTransform(const Gx::Matrix &value);

private:
    const VertexBuffer *mesh;
    RenderKey rk;
    Gx::Matrix tr;
};

#endif // STATICMESHNODE_H
