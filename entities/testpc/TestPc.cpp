#include "TestPc.h"

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

float bl[2] = { 0, 0 };

float clamp(float v){ return v < 0 ? 0 : (v > 1 ? 1 : v); }

}

TestPc::TestPc(Events &events, Graphics &graphics, Scene &scene) : kcc(0.45f, 2.0f, { 0, 1.14f, -0.5f }), ang(&events, Gx::Quaternion::identity())
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

void TestPc::update(const FrameParams &params, Events &events, Gx::PhysicsModel &physics, float delta)
{
    pos.store();
    time.store();

    auto m = Gx::Matrix::rotationY(params.camera.transform().angle().x);

    auto forw = Gx::Vec3(0, 0, 1).transformedNormal(m).normalized();
    auto right = Gx::Vec3(1, 0, 0).transformedNormal(m).normalized();

    float speed = 4.0f * delta;
    Gx::Vec3 step(0, 0, 0);

    if(!events.isKeyDown(VK_RBUTTON) && !events.isKeyDown(VK_SHIFT))
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
        ang.setRange(ang.value(), Gx::Quaternion::axisRotation({ 0, lookAngle(step.normalized()), 0 }), 0.1f);
    }

    float blMod[2] = { 0, 0 };

    if(events.isKeyDown('1')) blMod[0] = -1;
    if(events.isKeyDown('2')) blMod[0] = 1;

    if(events.isKeyDown('3')) blMod[1] = -1;
    if(events.isKeyDown('4')) blMod[1] = 1;

    for(int i = 0; i < 2; ++i)
    {
        bl[i] = clamp(bl[i] + blMod[i] * delta);
    }

    time.add(delta);
}

struct WeightedKey
{
    Gx::KeyFrame key;
    float weight;
};

void TestPc::prepareScene(SceneParams &params, float blend)
{
    auto bp = pos.value(blend);
    auto tr = ang.value(blend).matrix() * Gx::Matrix::translation(bp);

    params.objectDepthMatrix = Gx::Matrix::lookAt(bp + Gx::Vec3(0, 2, 0), bp + Gx::Vec3(0, -2, 0), Gx::Vec3(0, 0, 1)) * Gx::Matrix::ortho({ 2.2f, 2.2f }, { -100, 100 });

    node->updateTransform(tr);

    float t = std::fmod(time.value(blend), 1.0f);

    std::vector<WeightedKey> keys;

    float total = bl[0] + bl[1];

    if(total < 1.0f) keys.push_back({ anims["Idle"].keyFrame(t), 1.0f - total });
    if(bl[0]) keys.push_back({ anims["Walk"].keyFrame(t), bl[0] });
    if(bl[1]) keys.push_back({ anims["Run"].keyFrame(t), bl[1] });

    while(keys.size() > 1)
    {
        auto a = keys.back();
        keys.pop_back();

        auto b = keys.back();
        keys.pop_back();

        float total = a.weight + b.weight;
        auto c = Gx::Interpolator<Gx::KeyFrame>()(a.key, b.key, b.weight / total);

        keys.push_back({ c, total });
    }

    skeleton.setKeyFrame(keys.back().key);

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
