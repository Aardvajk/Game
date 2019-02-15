#ifndef STATE_H
#define STATE_H

class State
{
public:
    virtual ~State(){ }

    virtual bool update(float delta) = 0;
    virtual void render(float blend) = 0;
};

#endif // STATE_H
