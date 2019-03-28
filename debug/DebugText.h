#ifndef DEBUGTEXT_H
#define DEBUGTEXT_H

#include <GxMaths/GxColor.h>

#include <string>

class Graphics;

namespace DebugText
{

void acquire(Graphics &graphics);
void release();

int width(const std::string &text);
int height();

void draw(int x, int y, const std::string &text, const Gx::Color &color = { 0.9f, 0.9f, 0.9f });

}

#endif // DEBUGTEXT_H
