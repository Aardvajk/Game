#include "Pc.h"

#include "application/Events.h"

#include "scene/Scene.h"
#include "scene/nodes/StaticMeshNode.h"

#include "debug/DebugMesh.h"
#include "debug/DebugText.h"

#include <GxPhysics/GxBody.h>
#include <GxPhysics/GxShapes/GxCapsuleShape.h>

#include <GxCore/GxDebug.h>

Pc::Pc(Graphics &graphics, Scene &scene) : kcc(0.25f, 1.0f, { 0, 2, -0.5f })
{
    mesh = graphics.resources.add(new VertexBuffer(graphics.device, DebugMesh::capsuleToBuffer(16, 16, kcc.shape().radius(), kcc.shape().height(), Gx::Color(1.0f, 0.0f, 0.0f)), { }, Gx::Graphics::Pool::Managed));
    node = scene.addNode(new StaticMeshNode(mesh.get(), Gx::Matrix::translation(kcc.position())));

    pos.set(kcc.position());
}

void Pc::update(Events &events, Gx::PhysicsModel &physics, const Gx::Transform &camera, float delta)
{
    pos.store();

    Gx::Vec3 forw, right;
    camera.flatVectors(forw, right);

    float speed = 4.0f * delta;
    Gx::Vec3 step(0, 0, 0);

    if(!events.isKeyDown(VK_RBUTTON))
    {
        if(events.isKeyDown('W')) step += forw * speed;
        if(events.isKeyDown('S')) step -= forw * speed;

        if(events.isKeyDown('A')) step -= right * speed;
        if(events.isKeyDown('D')) step += right * speed;

        if(events.isKeyDown('R')) step.y += speed;
        if(events.isKeyDown('F')) step.y -= speed;
    }

    kcc.move(physics, step);
    pos.set(kcc.position());

    if(kcc.grounded()) DebugText() << "Grounded";
}

void Pc::prepareScene(SceneParams &params, float blend)
{
    node->updateTransform(Gx::Matrix::translation(pos.value(blend)));
}
