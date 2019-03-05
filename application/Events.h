#ifndef EVENTS_H
#define EVENTS_H

#include <GxMaths/GxVector.h>

#include <pcx/signals.h>

#include <unordered_set>

class Events
{
public:
    Events();

    Gx::Vec2 rawMouseDelta() const;
    bool isKeyDown(int key) const;

    pcx::signal<int> keyDown;
    pcx::signal<int> keyUp;

private:
    friend class Application;

    bool act;
    Gx::Vec2 rawMouse;
    std::unordered_set<int> keys;
};

#endif // EVENTS_H
