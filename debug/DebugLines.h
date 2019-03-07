#ifndef DEBUGLINES_H
#define DEBUGLINES_H

class Graphics;
class SceneParams;

namespace Gx
{

class Vec3;
class Color;

class PhysicsModel;

}

namespace DebugLines
{

void clear();
void addLine(const Gx::Vec3 &start, const Gx::Vec3 &end, const Gx::Color &color);

void addPhysics(const Gx::PhysicsModel &physics);

void render(Graphics &graphics, const SceneParams &params);

}

#endif // DEBUGLINES_H
