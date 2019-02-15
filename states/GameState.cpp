#include "GameState.h"

#include "graphics/Graphics.h"

#include <GxMaths/GxColor.h>

GameState::GameState(Graphics &graphics) : graphics(graphics)
{
}

bool GameState::update(float delta)
{
    return true;
}

void GameState::render(float blend)
{
    graphics.device.clear({ 0.2f, 0.5f, 0.8f }, 1.0f);
}
