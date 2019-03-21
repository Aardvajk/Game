#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "states/State.h"

#include "models/Model.h"

#include "scene/Scene.h"

#include "maths/Camera.h"

#include <GxPhysics/GxPhysicsModel.h>

#include <pcx/signals.h>

class Graphics;

class GameState : public State
{
public:
    explicit GameState(Events &events, Graphics &graphics);

    virtual bool update(AppParams &params, Events &events, float delta) override;
    virtual void render(Graphics &graphics, float blend) override;

private:
    void keyPressed(int key);

    Graphics &graphics;

    pcx::connections cx;

    Gx::PhysicsModel physics;
    Model model;
    Scene scene;
    Camera cam;

    bool drawPhysics;
    bool hasClosed;

    int shapes;
    float time;
};

#endif // GAMESTATE_H
