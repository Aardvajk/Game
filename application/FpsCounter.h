#ifndef FPSCOUNTER_H
#define FPSCOUNTER_H

class FpsCounter
{
public:
    FpsCounter();

    void update(float delta);
    void frame();

    int framesPerSecond() const { return fps; }

private:
    float time;
    int counter;
    int fps;
};

#endif // FPSCOUNTER_H
