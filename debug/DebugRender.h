#ifndef DEBUGRENDER_H
#define DEBUGRENDER_H

class Graphics;
class SceneParams;

namespace Gx
{

class Vec3;
class Color;
class Rect;
class Texture;
class PixelShader;
class PhysicsModel;

}

namespace DebugPoints
{

void clear();
void addPoint(const Gx::Vec3 &position, const Gx::Color &color);

void render(Graphics &graphics, const SceneParams &params);

}

namespace DebugLines
{

void clear();
void addLine(const Gx::Vec3 &start, const Gx::Vec3 &end, const Gx::Color &color);

void addPhysics(const Gx::PhysicsModel &physics);

void render(Graphics &graphics, const SceneParams &params);

}

namespace DebugRender
{

void drawScreenTexture(Graphics &graphics, const Gx::Rect &rect, const Gx::Texture &texture, const Gx::PixelShader *pixelShader);

}

#endif // DEBUGRENDER_H
