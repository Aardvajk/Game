#include "GameState.h"

#include <GxGraphics/GxGraphicsDevice.h>

#include <GxMaths/GxColor.h>

GameState::GameState(Gx::GraphicsDevice &device) : device(device)
{
}

bool GameState::update(float delta)
{
    return true;
}

void GameState::render(float blend)
{
    device.clear({ 0.2f, 0.5f, 0.8f }, 1.0f);
}
