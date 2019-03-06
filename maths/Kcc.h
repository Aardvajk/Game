#ifndef KCC_H
#define KCC_H

#include <GxPhysics/GxShapes/GxCapsuleShape.h>

namespace Gx
{

class PhysicsModel;

}

class Kcc
{
public:
    Kcc(float radius, float height, const Gx::Vec3 &position);

    void move(Gx::PhysicsModel &physics, const Gx::Vec3 &step);

    const Gx::CapsuleShape &shape() const;
    Gx::Vec3 position() const;
    bool grounded() const;

private:
    Gx::CapsuleShape sh;
    Gx::Vec3 pos;
    bool gr;
};

#endif // KCC_H
