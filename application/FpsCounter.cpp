#include "FpsCounter.h"

FpsCounter::FpsCounter() : time(0), counter(0), fps(0)
{
}

void FpsCounter::update(float delta)
{
    time += delta;
    if(time >= 1.0f)
    {
        fps = counter;

        time = 0;
        counter = 0;
    }
}

void FpsCounter::frame()
{
    ++counter;
}
