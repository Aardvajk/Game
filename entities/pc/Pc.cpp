#include "Pc.h"

#include "scene/Scene.h"
#include "scene/nodes/StaticMeshNode.h"

#include "debug/DebugMesh.h"
#include "debug/DebugText.h"

#include <GxPhysics/GxBody.h>

#include <GxPhysics/GxShapes/GxCapsuleShape.h>

#include <GxCore/GxDebug.h>

Pc::Pc(Graphics &graphics, Scene &scene) : kcc(0.25f, 1.0f, { 0, 2, -0.5f })
{
    mesh = graphics.resources.add(new VertexBuffer(graphics.device, debugCapsuleToBuffer(16, 16, kcc.shape().radius(), kcc.shape().height(), Gx::Color(1.0f, 0.0f, 0.0f)), { }, Gx::Graphics::Pool::Managed));
    node = scene.addNode(new StaticMeshNode(mesh.get(), Gx::Matrix::translation(kcc.position())));
}

void Pc::update(Gx::PhysicsModel &physics, const Gx::Transform &camera, float delta)
{
    Gx::Vec3 forw, right;
    camera.flatVectors(forw, right);

    float speed = 4.0f * delta;
    Gx::Vec3 step(0, 0, 0);

    if(!(GetAsyncKeyState(VK_RBUTTON) & 0x8000))
    {
        if(GetAsyncKeyState('W') & 0x8000) step += forw * speed;
        if(GetAsyncKeyState('S') & 0x8000) step -= forw * speed;

        if(GetAsyncKeyState('A') & 0x8000) step -= right * speed;
        if(GetAsyncKeyState('D') & 0x8000) step += right * speed;

        if(GetAsyncKeyState('R') & 0x8000) step.y += speed;
        if(GetAsyncKeyState('F') & 0x8000) step.y -= speed;
    }

    kcc.move(physics, step);
    node->updateTransform(Gx::Matrix::translation(kcc.position()));

    if(kcc.grounded()) DebugText() << "Grounded";
}
