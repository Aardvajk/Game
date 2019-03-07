#ifndef DEBUGRENDER_H
#define DEBUGRENDER_H

class Graphics;

namespace Gx
{

class Texture;

}

namespace DebugRender
{

void drawScreenTexture(Graphics &graphics, const Gx::Texture &texture);

}

#endif // DEBUGRENDER_H
