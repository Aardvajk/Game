#ifndef STATE_H
#define STATE_H

class AppParams;
class Events;
class Graphics;

class State
{
public:
    virtual ~State();

    virtual bool update(AppParams &params, Events &events, float delta) = 0;
    virtual void render(Graphics &graphics, float blend) = 0;
};

#endif // STATE_H
