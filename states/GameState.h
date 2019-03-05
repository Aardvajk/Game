#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "states/State.h"

#include "models/Model.h"

#include "scene/Scene.h"

#include <GxMaths/GxTransform.h>

#include <GxGraphics/GxVertexBuffer.h>

#include <GxPhysics/GxPhysicsModel.h>

class Pc;

class GameState : public State
{
public:
    explicit GameState(Graphics &graphics);
    virtual ~GameState() override;

    virtual bool update(Events &events, float delta) override;
    virtual void render(Graphics &graphics, float blend) override;

private:
    Gx::PhysicsModel physics;
    Model model;
    Scene scene;

    Gx::Transform cam;

    Pc *pc;
};

#endif // GAMESTATE_H
