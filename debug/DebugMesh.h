#ifndef DEBUGMESH_H
#define DEBUGMESH_H

#include <GxMaths/GxVector.h>
#include <GxMaths/GxColor.h>

namespace Gx
{

class VertexBuffer;

}

unsigned debugCuboidToBuffer(Gx::VertexBuffer &buffer, const Gx::Vec3 &dims, const Gx::Color &color);
unsigned debugCapsuleToBuffer(Gx::VertexBuffer &buffer, unsigned rings, unsigned segments, float radius, float height, const Gx::Color &color);

#endif // DEBUGMESH_H
