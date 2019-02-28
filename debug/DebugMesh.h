#ifndef DEBUGMESH_H
#define DEBUGMESH_H

#include <GxMaths/GxVector.h>
#include <GxMaths/GxColor.h>

#include <pcx/buffer.h>

pcx::buffer debugCuboidToBuffer(const Gx::Vec3 &dims, const Gx::Color &color);
pcx::buffer debugCapsuleToBuffer(unsigned rings, unsigned segments, float radius, float height, const Gx::Color &color);

#endif // DEBUGMESH_H
