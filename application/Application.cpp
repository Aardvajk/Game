#include "Application.h"

#include "states/GameState.h"

#include <GxCore/GxTimer.h>

#include <GxMaths/GxColor.h>

Application::Application(const Gx::DisplaySettings &settings) : Gx::Application(settings.size), graphics(hwnd(), settings)
{
    state = new GameState(graphics);
    show();
}

int Application::exec()
{
    Gx::Timer timer;

    const float delta = 1.0f / 60.0f;
    float accumulator = delta;

    while(loop())
    {
        if(!graphics.device.isOk())
        {
            if(graphics.device.isReadyToReset())
            {
                graphics.reset();
            }
        }
    
        if(graphics.device.isOk())
        {
            accumulator += timer.elapsed(Gx::Timer::Flag::Restart);
            update(accumulator, delta);
        }
    }
    
    return 0;
}

void Application::update(float &accumulator, float delta)
{
    while(accumulator >= delta)
    {
        if(!state->update(delta))
        {
            close();
            return;
        }

        accumulator -= delta;
    }

    graphics.device.begin();
    state->render(accumulator / delta);
    graphics.device.end();
}
