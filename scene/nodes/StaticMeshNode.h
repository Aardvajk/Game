#ifndef STATICMESHNODE_H
#define STATICMESHNODE_H

#include "scene/nodes/SceneNode.h"

#include <GxMaths/GxMatrix.h>

class VertexBuffer;

class StaticMeshNode : public SceneNode
{
public:
    StaticMeshNode(const VertexBuffer *mesh, const Gx::Matrix &transform);

    virtual void render(Graphics &graphics) const override;

private:
    const VertexBuffer *mesh;
    Gx::Matrix tr;
};

#endif // STATICMESHNODE_H
