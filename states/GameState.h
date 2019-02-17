#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "states/State.h"

#include "graphics/Graphics.h"

#include <GxMaths/GxTransform.h>

#include <GxGraphics/GxVertexBuffer.h>

#include <pcx/signals.h>

class GameState : public State
{
public:
    GameState(Graphics &graphics);

    virtual bool update(float delta) override;
    virtual void render(float blend) override;

private:
    void deviceReset();

    pcx::connections cx;

    Graphics &graphics;

    Graphics::Handle<Gx::VertexBuffer> cuboid;
    unsigned cuboidCount;

    Graphics::Handle<Gx::VertexBuffer> capsule;
    unsigned capsuleCount;

    Gx::Transform cam;
    Gx::Vec2 prevMouse;
};

#endif // GAMESTATE_H
