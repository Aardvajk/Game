#ifndef ENTITY_H
#define ENTITY_H

#include <pcx/non_copyable.h>

class FrameParams;
class Events;
class SceneParams;

namespace Gx
{

class PhysicsModel;

}

class Entity : public pcx::non_copyable
{
public:
    virtual ~Entity();

    virtual void update(const FrameParams &params, Events &events, Gx::PhysicsModel &physics, float delta) = 0;
    virtual void prepareScene(const SceneParams &params, float blend) = 0;
};

#endif // ENTITY_H
