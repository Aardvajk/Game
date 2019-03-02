#include "Pc.h"

#include "scene/Scene.h"
#include "scene/nodes/StaticMeshNode.h"

#include "debug/DebugMesh.h"
#include "debug/DebugRender.h"

#include <GxPhysics/GxBody.h>

#include <GxPhysics/GxShapes/GxCapsuleShape.h>

#include <GxCore/GxDebug.h>

Pc::Pc(Graphics &graphics, Scene &scene)
{
    mesh = graphics.resources.add(new VertexBuffer(graphics.device, debugCapsuleToBuffer(16, 17, 0.25f, 1.0f, Gx::Color(1.0f, 0.0f, 0.0f)), { }, Gx::Graphics::Pool::Managed));
    pos = Gx::Vec3(0, 2, -0.5f);

    node = scene.addNode(new StaticMeshNode(mesh.get(), Gx::Matrix::translation(pos)));
}

void Pc::update(Gx::PhysicsModel &physics, const Gx::Transform &camera, float delta)
{
    Gx::Vec3 forw, right;
    camera.flatVectors(forw, right);

    float speed = 2.0f * delta;

    if(!(GetAsyncKeyState(VK_RBUTTON) & 0x8000))
    {
        if(GetAsyncKeyState('W') & 0x8000) pos += forw * speed;
        if(GetAsyncKeyState('S') & 0x8000) pos -= forw * speed;

        if(GetAsyncKeyState('A') & 0x8000) pos -= right * speed;
        if(GetAsyncKeyState('D') & 0x8000) pos += right * speed;

        if(GetAsyncKeyState('R') & 0x8000) pos.y += speed;
        if(GetAsyncKeyState('F') & 0x8000) pos.y -= speed;
    }

    Gx::CapsuleShape shape(0.25f, 1.0f);
    Gx::Matrix to = Gx::Matrix::translation(pos);

    auto r = physics.broadphase(shape.aabb(to));

    if(!r.empty())
    {
        int it = 0;
        bool loop = true;

        while(it < 5 && loop)
        {
            loop = false;

            for(auto &b: r)
            {
                auto r = physics.convexIntersection(shape, to, b->shape(), b->transform());

                if(r)
                {
                    pos += r->separatingVector;
                    to = Gx::Matrix::translation(pos);

                    loop = true;
                }
            }

            ++it;
        }
    }

    node->updateTransform(Gx::Matrix::translation(pos));

    auto ray = pos + Gx::Vec3(0, -0.5f, 0);
    if(auto r = physics.rayCast(Gx::Ray(ray, Gx::Vec3(0, -1, 0)), 100.0f))
    {
        DebugRender::addLine(ray, r->point, Gx::Color(1, 1, 1));
    }
}
