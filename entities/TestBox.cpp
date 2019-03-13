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

float randomColor()
{
    return (std::rand() % 100) / 100.0f;
}

TestBox::TestBox(Graphics &graphics, Scene &scene, Gx::PhysicsModel &physics, const Gx::Vec3 &dims, const Gx::Vec3 &position) : tr(Gx::Transform(position, { 0, 0 }))
{
    int type = std::rand() % 4;

    if(type == 0)
    {
        mesh = graphics.resources.add(new VertexBuffer(graphics.device, DebugMesh::flatMesh(DebugMesh::cuboid(dims), Gx::Color(randomColor(), randomColor(), randomColor())), { }, Gx::Graphics::Pool::Managed));
        body = physics.createBody(new Gx::PolyhedronShape(dims), Gx::Matrix::translation(position), 1.0f);
    }
    else if(type == 1)
    {
        float radius = dims.x * 0.5f;
        float height = radius * (3 + (std::rand() % 2));

        mesh = graphics.resources.add(new VertexBuffer(graphics.device, DebugMesh::smoothMesh(DebugMesh::capsule(16, 16, radius, height), Gx::Color(randomColor(), randomColor(), randomColor())), { }, Gx::Graphics::Pool::Managed));
        body = physics.createBody(new Gx::CapsuleShape(radius, height), Gx::Matrix::translation(position), 1.0f);
    }
    else if(type == 2)
    {
        float radius = dims.x * 0.5f;

        mesh = graphics.resources.add(new VertexBuffer(graphics.device, DebugMesh::smoothMesh(DebugMesh::capsule(16, 16, radius, radius * 2), Gx::Color(randomColor(), randomColor(), randomColor())), { }, Gx::Graphics::Pool::Managed));
        body = physics.createBody(new Gx::SphereShape(radius), Gx::Matrix::translation(position), 1.0f);
    }
    else if(type == 3)
    {
        float radius = dims.x * 0.5f;

        std::vector<Gx::Vec3> vs = { { radius, radius, radius }, { -radius,  radius, -radius }, { radius, -radius, -radius }, { -radius, -radius, radius } };
        std::vector<Gx::PolyhedronShape::Face> fs = { { 1, 2, 3 }, { 0, 3, 2 }, { 0, 1, 3 }, { 0, 2, 1 } };

        mesh = graphics.resources.add(new VertexBuffer(graphics.device, DebugMesh::flatMesh(DebugMesh::tetrahedron(radius), Gx::Color(randomColor(), randomColor(), randomColor())), { }, Gx::Graphics::Pool::Managed));
        body = physics.createBody(new Gx::PolyhedronShape(vs, fs), Gx::Matrix::translation(position), 1.0f);
    }

    node = scene.addNode(new StaticMeshNode(mesh.get(), Gx::Matrix::translation(position)));
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
