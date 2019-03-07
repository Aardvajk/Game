#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "states/State.h"

#include "models/Model.h"

#include "scene/Scene.h"

#include "maths/Camera.h"

#include <GxPhysics/GxPhysicsModel.h>

class Graphics;
class Pc;

class GameState : public State
{
public:
    explicit GameState(Graphics &graphics);
    virtual ~GameState() override;

    virtual bool update(AppParams &params, Events &events, float delta) override;
    virtual void render(Graphics &graphics, float blend) override;

private:
    Gx::PhysicsModel physics;
    Model model;
    Scene scene;

    Camera cam;

    Pc *pc;
};

#endif // GAMESTATE_H
