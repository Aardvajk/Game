#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "states/State.h"

#include "graphics/Graphics.h"

#include "models/Model.h"

#include "scene/Scene.h"

#include <GxMaths/GxTransform.h>

#include <GxGraphics/GxVertexBuffer.h>

#include <GxPhysics/GxPhysicsModel.h>

class GameState : public State
{
public:
    explicit GameState(Graphics &graphics);

    virtual bool update(float delta) override;
    virtual void render(float blend) override;

private:
    Graphics &graphics;

    Gx::PhysicsModel physics;
    Model model;
    Scene scene;

    Gx::Transform cam;
    Gx::Vec2 prevMouse;
};

#endif // GAMESTATE_H
