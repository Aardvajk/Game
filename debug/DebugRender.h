#ifndef DEBUGRENDER_H
#define DEBUGRENDER_H

class Graphics;

namespace Gx
{

class Rect;
class Texture;
class PixelShader;

}

namespace DebugRender
{

void drawScreenTexture(Graphics &graphics, const Gx::Rect &rect, const Gx::Texture &texture, const Gx::PixelShader *pixelShader);

}

#endif // DEBUGRENDER_H
