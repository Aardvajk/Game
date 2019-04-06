#ifndef TESTSHAPE_H
#define TESTSHAPE_H

#include "entities/Entity.h"

#include "graphics/Graphics.h"

#include <GxMaths/GxBlendValue.h>
#include <GxMaths/GxVector.h>
#include <GxMaths/GxTransform.h>

#include <pcx/scoped_ptr.h>

class Scene;
class StaticMeshNode;

namespace Gx
{

class PhysicsModel;
class Body;

}

class TestShape : public Entity
{
public:
    TestShape(Graphics &graphics, Scene &scene, Gx::PhysicsModel &physics, const Gx::Vec3 &dims, const Gx::Vec3 &position);
    virtual ~TestShape() override;

    virtual void update(const FrameParams &params, Events &events, Gx::PhysicsModel &physics, float delta) override;
    virtual void prepareScene(SceneParams &params, float blend) override;

    Gx::Body *physicsBody(){ return body.get(); }

private:
    Graphics::Handle<VertexBuffer> mesh;
    pcx::scoped_ptr<StaticMeshNode> node;
    pcx::scoped_ptr<Gx::Body> body;
    Gx::BlendTransform tr;
};

#endif // TESTSHAPE_H
