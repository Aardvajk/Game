#include "Events.h"

Events::Events() : act(true), rawMouse(0, 0), tm(0)
{
}

void Events::update(float delta)
{
    tm += delta;
}

Gx::Vec2 Events::rawMouseDelta() const
{
    return rawMouse;
}

bool Events::isKeyDown(int key) const
{
    return keys.find(key) != keys.end();
}

float Events::time() const
{
    return tm;
}
