#ifndef DEBUGRENDER_H
#define DEBUGRENDER_H

class Graphics;

namespace Gx
{

class Rect;
class Texture;

}

namespace DebugRender
{

void drawScreenTexture(Graphics &graphics, const Gx::Rect &rect, const Gx::Texture &texture);

}

#endif // DEBUGRENDER_H
