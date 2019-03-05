#ifndef APPLICATION_H
#define APPLICATION_H

#include "application/Events.h"

#include "graphics/Graphics.h"

#include "states/State.h"

#include <GxApplication/GxApplication.h>

#include <pcx/scoped_ptr.h>

class Application : public Gx::Application
{
public:
    Application(const Gx::DisplaySettings &settings);

    int exec();

protected:
    virtual void activationEvent(bool state) override;
    virtual void rawMouseEvent(int x, int y) override;
    virtual void keyPressedEvent(int key) override;
    virtual void keyReleasedEvent(int key) override;

private:
    void update(float &accumulator, float delta);

    Events events;
    Graphics graphics;
    pcx::scoped_ptr<State> state;
};

#endif // APPLICATION_H
