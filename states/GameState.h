#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "states/State.h"

#include "models/Model.h"

class Graphics;

class GameState : public State
{
public:
    GameState(Graphics &graphics);

    virtual bool update(float delta) override;
    virtual void render(float blend) override;

private:
    Graphics &graphics;
    Model model;
};

#endif // GAMESTATE_H
