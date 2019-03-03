#ifndef DEBUGTEXT_H
#define DEBUGTEXT_H

#include <sstream>

class Graphics;

class DebugText
{
public:
    DebugText() = default;
    ~DebugText();

    template<typename T> DebugText &operator<<(const T &v){ s << v; return *this; }

    static void init(Graphics &graphics);
    static void release();

    static void clear();
    static void render(Graphics &graphics);

private:
    std::ostringstream s;
};

#endif // DEBUGTEXT_H
