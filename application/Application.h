#ifndef APPLICATION_H
#define APPLICATION_H

#include "application/AppParams.h"
#include "application/Events.h"
#include "application/FpsCounter.h"

#include "graphics/Graphics.h"

#include <GxApplication/GxApplication.h>

#include <pcx/scoped_ptr.h>

class State;

class Application : public Gx::Application
{
public:
    Application(const Gx::DisplaySettings &settings);
    virtual ~Application() override;

    int exec();

protected:
    virtual void activationEvent(bool state) override;
    virtual void rawMouseEvent(int x, int y) override;
    virtual void keyPressedEvent(int key) override;
    virtual void keyReleasedEvent(int key) override;

private:
    void update(float &accumulator, float delta);
    void renderDebugInfo();

    AppParams params;
    Events events;
    Graphics graphics;
    pcx::scoped_ptr<State> state;

    FpsCounter fpsCounter;
};

#endif // APPLICATION_H
