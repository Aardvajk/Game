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
#include <GxMaths/GxQuaternion.h>

#include <GxPhysics/GxBody.h>
#include <GxPhysics/GxShapes/GxCapsuleShape.h>
#include <GxPhysics/GxShapes/GxSphereShape.h>

#include <GxAnimation/GxSkeletonJoint.h>

#include <pcx/datastream.h>

#include <cmath>

namespace
{

float lookAngle(const Gx::Vec3 &v)
{
    float a = std::acos(Gx::Vec3(0, 0, 1).dot(v));
    if(v.x < 0)
    {
        a *= -1;
    }

    return a + float(M_PI);
}

pcx::data_istream &operator>>(pcx::data_istream &ds, Gx::Vec3 &v)
{
    return ds >> v.x >> v.y >> v.z;
}

pcx::data_istream &operator>>(pcx::data_istream &ds, Gx::Quaternion &q)
{
    return ds >> q.x >> q.y >> q.z >> q.w;
}

pcx::data_istream &operator>>(pcx::data_istream &ds, Gx::JointTransform &t)
{
    return ds >> t.rotation >> t.translation;
}

pcx::data_istream &operator>>(pcx::data_istream &ds, Gx::KeyFrame &k)
{
    k.position = ds.get<float>();
    k.transforms = ds.get<std::vector<Gx::JointTransform> >();

    return ds;
}

pcx::data_istream &operator>>(pcx::data_istream &ds, Gx::AnimationEvent &e)
{
    auto position = ds.get<float>();
    auto data = ds.get<std::string>();

    e = Gx::AnimationEvent(position, data);

    return ds;
}

}

Pc::Pc(Graphics &graphics, Scene &scene) : kcc(0.45f, 2.0f, { 0, 1.14f, -0.5f })
{
    pcx::data_ifstream ds(resourcePath("assets/models/model.dat"));

    mesh = graphics.resources.add(new VertexBuffer(graphics.device, ds, { }, Gx::Graphics::Pool::Managed));
    node = scene.addNode(new RigMeshNode(mesh.get(), Gx::Matrix::translation(kcc.position())));

    pos.set(kcc.position());

    auto joints = ds.get<std::size_t>();
    for(std::size_t i = 0; i < joints; ++i)
    {
        auto name = ds.get<std::string>();
        auto offset = ds.get<Gx::Vec3>();

        pcx::optional<std::uint8_t> parent;

        auto p = ds.get<std::uint8_t>();
        if(p != static_cast<std::uint8_t>(-1))
        {
            parent = p;
        }

        skeleton.addJoint(name, { offset, parent });
    }

    auto animations = ds.get<std::size_t>();
    for(std::size_t i = 0; i < animations; ++i)
    {
        auto name = ds.get<std::string>();

        auto count = ds.get<std::size_t>();
        auto duration = ds.get<float>();

        auto keys = ds.get<std::vector<Gx::KeyFrame> >();
        auto events = ds.get<std::vector<Gx::AnimationEvent> >();

        anims[name] = Gx::Animation(count, duration, keys, events, true);
    }
}

void Pc::update(const FrameParams &params, Events &events, Gx::PhysicsModel &physics, float delta)
{
    pos.store();
    ang.store();
    time.store();

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

    if(step.length())
    {
        ang.set(lookAngle(step.normalized()));
    }

    time.add(delta);
}

void Pc::prepareScene(SceneParams &params, float blend)
{
    auto bp = pos.value(blend);
    auto ba = ang.value(blend);

    params.objectDepthMatrix = Gx::Matrix::lookAt(bp + Gx::Vec3(0, 2, 0), bp + Gx::Vec3(0, -2, 0), Gx::Vec3(0, 0, 1)) * Gx::Matrix::ortho({ 1.0f, 1.0f }, { -100, 100 });

    auto tr = Gx::Matrix::rotationY(ba) * Gx::Matrix::translation(bp);

    node->updateTransform(tr);

    auto &a = anims["Run"];

    skeleton.setKeyFrame(a.keyFrame(time.value(blend) / a.duration()));
    node->updatePalette(skeleton.palette());

    if(params.drawSkeleton)
    {
        for(std::size_t i = 0; i < skeleton.size(); ++i)
        {
            auto j = skeleton.joint(i);
            if(j.parent)
            {
                DebugLines::addLine(j.offset.transformedCoord(skeleton.transform(i) * tr), skeleton.joint(*j.parent).offset.transformedCoord(skeleton.transform(*j.parent) * tr), { 1, 0, 0 });
            }
        }

        for(std::size_t i = 0; i < skeleton.size(); ++i)
        {
            DebugLines::addShape(Gx::SphereShape(0.05f), Gx::Matrix::translation(skeleton.joint(i).offset) * skeleton.transform(i) * tr, { 0, 1, 0 });
        }
    }

    if(params.drawPhysics)
    {
        DebugLines::addShape(Gx::CapsuleShape(0.45f, 2.0f), Gx::Matrix::translation(bp), { 0, 1, 1 });
    }
}
