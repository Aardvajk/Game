#include "Application.h"

#include "states/GameState.h"

#include "debug/DebugText.h"
#include "debug/MemUsage.h"

#include <GxCore/GxTimer.h>

#include <GxMaths/GxColor.h>

#include <pcx/str.h>

Application::Application(const Gx::Point &position, const Gx::DisplaySettings &settings) : Gx::Application(position, settings.size), params{ settings.size }, graphics(hwnd(), settings)
{
    DebugText::acquire(graphics);

    state = new GameState(events, graphics);
    show();
}

Application::~Application()
{
    DebugText::release();
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

void Application::activationEvent(bool state)
{
    events.act = state;
    if(!state)
    {
        events.keys.clear();
    }
}

void Application::rawMouseEvent(int x, int y)
{
    events.rawMouse += Gx::Vec2(static_cast<float>(x), static_cast<float>(y));
}

void Application::keyPressedEvent(int key)
{
    if(events.act)
    {
        events.keys.insert(key);
        events.keyDown(key);
    }
}

void Application::keyReleasedEvent(int key)
{
    events.keys.erase(key);
}

void Application::update(float &accumulator, float delta)
{
    while(accumulator >= delta)
    {
        events.update(delta);
        fps.update(delta);

        if(!state->update(params, events, delta))
        {
            close();
            return;
        }

        events.rawMouse = Gx::Vec2(0, 0);
        accumulator -= delta;
    }

    fps.frame();

    graphics.device.begin();

    state->render(graphics, accumulator / delta);
    renderDebugInfo();

    graphics.device.end();
}

void Application::renderDebugInfo()
{
    auto s = pcx::str("FPS:", fps.framesPerSecond(), " Mem:", MemUsage::current(), "mb Peak:", MemUsage::peak(), "mb");

    DebugText::draw(params.size.width - (DebugText::width(s) + 2), params.size.height - (DebugText::height() + 2), s);
}
