#include "Events.h"

Events::Events() : act(true), rawMouse(0, 0)
{
}

Gx::Vec2 Events::rawMouseDelta() const
{
    return rawMouse;
}

bool Events::isKeyDown(int key) const
{
    return keys.find(key) != keys.end();
}
