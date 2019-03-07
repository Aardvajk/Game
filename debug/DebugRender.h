#ifndef DEBUGRENDER_H
#define DEBUGRENDER_H

#include <GxMaths/GxRect.h>

class Graphics;

namespace Gx
{

class Texture;

}

namespace DebugRender
{

void drawScreenTexture(Graphics &graphics, const Gx::Rect &rect, const Gx::Texture &texture);

}

#endif // DEBUGRENDER_H
