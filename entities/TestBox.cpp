#include "TestBox.h"

#include "graphics/VertexBuffer.h"

#include "scene/Scene.h"
#include "scene/nodes/StaticMeshNode.h"

#include "debug/DebugMesh.h"

#include <GxPhysics/GxPhysicsModel.h>
#include <GxPhysics/GxBody.h>
#include <GxPhysics/GxShapes/GxPolyhedronShape.h>
#include <GxPhysics/GxShapes/GxCapsuleShape.h>
#include <GxPhysics/GxShapes/GxSphereShape.h>
#include <GxPhysics/GxShapes/GxConeShape.h>

TestBox::TestBox(Graphics &graphics, Scene &scene, Gx::PhysicsModel &physics, const Gx::Vec3 &dims, const Gx::Vec3 &position) : tr(Gx::Transform(position, { 0, 0 }))
{
    auto r = [](){ return (std::rand() % 100) / 100.0f; };

    int type = std::rand() % 5;
    float mass = 0.2f;

    if(type == 0)
    {
        auto m = DebugMesh::cuboid(dims);

        mesh = graphics.resources.add(new VertexBuffer(graphics.device, DebugMesh::flatMesh(m, Gx::Color(r(), r(), r())), { }, Gx::Graphics::Pool::Managed));
        body = physics.createBody(new Gx::PolyhedronShape(m.vs, m.fs), Gx::Matrix::translation(position), mass);
    }
    else if(type == 1)
    {
        float radius = dims.x * 0.5f;
        float height = radius * (3 + (std::rand() % 2));

        mesh = graphics.resources.add(new VertexBuffer(graphics.device, DebugMesh::smoothMesh(DebugMesh::capsule(16, 16, radius, height), Gx::Color(r(), r(), r())), { }, Gx::Graphics::Pool::Managed));
        body = physics.createBody(new Gx::CapsuleShape(radius, height), Gx::Matrix::translation(position), mass);
    }
    else if(type == 2)
    {
        float radius = dims.x * 0.5f;

        mesh = graphics.resources.add(new VertexBuffer(graphics.device, DebugMesh::smoothMesh(DebugMesh::sphere(16, 16, radius), Gx::Color(r(), r(), r())), { }, Gx::Graphics::Pool::Managed));
        body = physics.createBody(new Gx::SphereShape(radius), Gx::Matrix::translation(position), 1.0f);
    }
    else if(type == 3)
    {
        auto m = DebugMesh::tetrahedron(dims.x * 0.5f);

        mesh = graphics.resources.add(new VertexBuffer(graphics.device, DebugMesh::flatMesh(m, Gx::Color(r(), r(), r())), { }, Gx::Graphics::Pool::Managed));
        body = physics.createBody(new Gx::PolyhedronShape(m.vs, m.fs), Gx::Matrix::translation(position), mass);
    }
    else if(type == 4)
    {
        float radius = dims.x * 0.5f;
        float height = radius * (2 + (std::rand() % 2));

        mesh = graphics.resources.add(new VertexBuffer(graphics.device, DebugMesh::mixedMesh(DebugMesh::cone(16, radius, height), Gx::Color(r(), r(), r())), { }, Gx::Graphics::Pool::Managed));
        body = physics.createBody(new Gx::ConeShape(radius, height), Gx::Matrix::translation(position), mass);
    }

    node = scene.addNode(new StaticMeshNode(mesh.get(), { }, Gx::Matrix::translation(position)));
}

TestBox::~TestBox()
{
}

void TestBox::update(const FrameParams &params, Events &events, Gx::PhysicsModel &physics, float delta)
{
    tr.store();
    tr.set(body->transform());
}

void TestBox::prepareScene(SceneParams &params, float blend)
{
    auto t = tr.value(blend);
    node->updateTransform(t.rotation().matrix() * Gx::Matrix::translation(t.position()));
}
