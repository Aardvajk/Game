#ifndef PC_H
#define PC_H

#include "entities/Entity.h"

#include "maths/Kcc.h"

#include "graphics/Graphics.h"

#include <GxMaths/GxBlendValue.h>
#include <GxMaths/GxVector.h>

#include <pcx/scoped_ptr.h>

class Scene;
class RigMeshNode;
class PcNode;

class Pc : public Entity
{
public:
    Pc(Graphics &graphics, Scene &scene);

    virtual void update(const FrameParams &params, Events &events, Gx::PhysicsModel &physics, float delta) override;
    virtual void prepareScene(SceneParams &params, float blend) override;

private:
    Graphics::Handle<VertexBuffer> mesh;
    pcx::scoped_ptr<RigMeshNode> node;

    Kcc kcc;
    Gx::BlendVec3 pos;
};

#endif // PC_H
