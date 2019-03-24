#ifndef PC_H
#define PC_H

#include "entities/Entity.h"

#include "maths/Kcc.h"

#include "graphics/Graphics.h"

#include <GxMaths/GxBlendValue.h>
#include <GxMaths/GxVector.h>

#include <GxAnimation/GxSkeleton.h>
#include <GxAnimation/GxAnimation.h>

#include <pcx/scoped_ptr.h>

#include <unordered_map>

class Scene;
class RigMeshNode;

class Pc : public Entity
{
public:
    Pc(Graphics &graphics, Scene &scene);

    virtual void update(const FrameParams &params, Events &events, Gx::PhysicsModel &physics, float delta) override;
    virtual void prepareScene(SceneParams &params, float blend) override;

private:
    Graphics::Handle<VertexBuffer> mesh;
    pcx::scoped_ptr<RigMeshNode> node;

    Kcc kcc;
    Gx::Skeleton skeleton;

    std::unordered_map<std::string, Gx::Animation> anims;

    Gx::BlendVec3 pos;
    Gx::BlendFloat ang;

    Gx::BlendFloat time;
};

#endif // PC_H
