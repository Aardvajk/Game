#ifndef DEBUGTEXT_H
#define DEBUGTEXT_H

#include <string>

class Graphics;

namespace DebugText
{

void acquire(Graphics &graphics);
void release();

int width(const std::string &text);
int height();

void draw(int x, int y, const std::string &text);

}

#endif // DEBUGTEXT_H
