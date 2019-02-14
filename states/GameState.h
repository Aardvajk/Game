#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "states/State.h"

#include "models/Model.h"

namespace Gx
{

class GraphicsDevice;

}

class GameState : public State
{
public:
    GameState(Gx::GraphicsDevice &device);

    virtual bool update(float delta) override;
    virtual void render(float blend) override;

private:
    Gx::GraphicsDevice &device;
    Model model;
};

#endif // GAMESTATE_H
