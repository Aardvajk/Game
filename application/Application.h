#ifndef APPLICATION_H
#define APPLICATION_H

#include "states/State.h"

#include <GxApplication/GxApplication.h>

#include <GxGraphics/GxExclusiveGraphicsDevice.h>

#include <pcx/scoped_ptr.h>

class Application : public Gx::Application
{
public:
    Application(const Gx::DisplaySettings &settings);

    int exec();

private:
    void update(float &accumulator, float delta);

    Gx::ExclusiveGraphicsDevice device;
    pcx::scoped_ptr<State> state;
};

#endif // APPLICATION_H
