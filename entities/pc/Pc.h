#ifndef PC_H
#define PC_H

#include "entities/Entity.h"

#include "maths/Kcc.h"

#include "graphics/Graphics.h"

#include <GxMaths/GxBlendValue.h>
#include <GxMaths/GxVector.h>

#include <pcx/scoped_ptr.h>

class Events;
class Physics;
class Scene;
class SceneParams;
class StaticMeshNode;

namespace Gx
{

class PhysicsModel;
class Transform;

}

class Pc : public Entity
{
public:
    Pc(Graphics &graphics, Scene &scene);

    void update(Events &events, Gx::PhysicsModel &physics, const Gx::Transform &camera, float delta);
    void prepareScene(SceneParams &params, float blend);

private:
    Graphics::Handle<VertexBuffer> mesh;
    pcx::scoped_ptr<StaticMeshNode> node;

    Kcc kcc;
    Gx::BlendVec3 pos;
};

#endif // PC_H
