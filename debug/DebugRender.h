#ifndef DEBUGRENDER_H
#define DEBUGRENDER_H

#include <GxMaths/GxVector.h>
#include <GxMaths/GxColor.h>

#include <pcx/aligned_store.h>

class Graphics;
class SceneParams;

namespace Gx
{

class PhysicsModel;

}

namespace DebugRender
{

void clear();
void addLine(const Gx::Vec3 &start, const Gx::Vec3 &end, const Gx::Color &color);

void addPhysics(const Gx::PhysicsModel &physics);

void render(Graphics &graphics, const SceneParams &params);

}

#endif // DEBUGRENDER_H
