#ifndef SKYBOXNODE_H
#define SKYBOXNODE_H

#include "scene/nodes/SceneNode.h"

class VertexBuffer;

namespace Gx
{

class CubeMap;

}

class SkyBoxNode : public SceneNode
{
public:
    explicit SkyBoxNode(const VertexBuffer *mesh, const Gx::CubeMap *diffuse);

    virtual bool pass(RenderPass type) const override;
    virtual RenderType type() const override;
    virtual RenderKey key() const override;

    virtual void render(RenderPass pass, Graphics &graphics, SceneParams &params) const override;

private:
    const VertexBuffer *mesh;
    const Gx::CubeMap *diffuse;
};

#endif // SKYBOXNODE_H
