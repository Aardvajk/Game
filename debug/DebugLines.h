#ifndef DEBUGLINES_H
#define DEBUGLINES_H

#include <GxMaths/GxVector.h>
#include <GxMaths/GxColor.h>

#include <pcx/aligned_store.h>

class Graphics;
class SceneParams;

namespace Gx
{

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
