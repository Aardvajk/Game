#ifndef PC_H
#define PC_H

#include "entities/Entity.h"

#include "graphics/Graphics.h"

#include <GxMaths/GxVector.h>
#include <GxMaths/GxTransform.h>

#include <GxPhysics/GxPhysicsModel.h>

#include <pcx/scoped_ptr.h>

class Scene;
class StaticMeshNode;
class Physics;

class Pc : public Entity
{
public:
    Pc(Graphics &graphics, Scene &scene);

    void update(Gx::PhysicsModel &physics, const Gx::Transform &camera, float delta);

private:
    Graphics::Handle<VertexBuffer> mesh;
    Gx::Vec3 pos;

    pcx::scoped_ptr<StaticMeshNode> node;
};

#endif // PC_H
