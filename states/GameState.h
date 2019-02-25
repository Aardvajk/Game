#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "states/State.h"

#include "graphics/Graphics.h"

#include <GxMaths/GxTransform.h>

#include <GxGraphics/GxVertexBuffer.h>

#include <GxPhysics/GxPhysicsModel.h>

#include <pcx/signals.h>

class GameState : public State
{
public:
    explicit GameState(Graphics &graphics);

    virtual bool update(float delta) override;
    virtual void render(float blend) override;

private:
    void deviceReset();

    pcx::connections cx;

    Graphics &graphics;
    Graphics::Handle<Gx::VertexBuffer> buffer;
    unsigned count;

    Gx::PhysicsModel physics;

    Gx::Transform cam;
    Gx::Vec2 prevMouse;
};

#endif // GAMESTATE_H
