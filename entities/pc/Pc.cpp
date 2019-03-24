#include "Pc.h"

#include "application/FrameParams.h"
#include "application/Events.h"
#include "application/ResourcePath.h"

#include "graphics/VertexBuffer.h"

#include "scene/Scene.h"
#include "scene/SceneParams.h"

#include "scene/nodes/RigMeshNode.h"

#include "debug/DebugMesh.h"
#include "debug/DebugRender.h"

#include <GxMaths/GxVector.h>
#include <GxMaths/GxMatrix.h>
#include <GxMaths/GxTransform.h>
#include <GxMaths/GxRange.h>

#include <GxPhysics/GxBody.h>
#include <GxPhysics/GxShapes/GxCapsuleShape.h>

#include <pcx/datastream.h>

Pc::Pc(Graphics &graphics, Scene &scene) : kcc(0.45f, 2.0f, { 0, 1.14f, -0.5f })
{
    pcx::data_ifstream ds(resourcePath("assets/models/model.dat"));

    mesh = graphics.resources.add(new VertexBuffer(graphics.device, ds, { }, Gx::Graphics::Pool::Managed));
    node = scene.addNode(new RigMeshNode(mesh.get(), Gx::Matrix::translation(kcc.position())));

    pos.set(kcc.position());
}

void Pc::update(const FrameParams &params, Events &events, Gx::PhysicsModel &physics, float delta)
{
    pos.store();

    Gx::Vec3 forw, right;
    params.camera.transform().flatVectors(forw, right);

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
}

void Pc::prepareScene(SceneParams &params, float blend)
{
    auto bp = pos.value(blend);
    params.objectDepthMatrix = Gx::Matrix::lookAt(bp + Gx::Vec3(0, 2, 0), bp + Gx::Vec3(0, -2, 0), Gx::Vec3(0, 0, 1)) * Gx::Matrix::ortho({ 1.0f, 1.0f }, { -100, 100 });

    node->updateTransform(Gx::Matrix::translation(bp));

    if(params.drawPhysics)
    {
        DebugLines::addShape(Gx::CapsuleShape(0.45f, 2.0f), Gx::Matrix::translation(bp));
    }
}
