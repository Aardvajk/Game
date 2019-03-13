#include "Kcc.h"

#include <GxMaths/GxMatrix.h>
#include <GxMaths/GxQuaternion.h>
#include <GxMaths/GxColor.h>

#include <GxPhysics/GxPhysicsModel.h>
#include <GxPhysics/GxBody.h>

#include <pcx/optional.h>

#include <utility>

namespace
{

float sphericalDistanceToNormal(float radius, const Gx::Vec3 &n)
{
    float d = Gx::Vec3(0, 1, 0).dot(n);
    return d ? radius / d : radius;
}

float minFloorDistance(float radius, const Gx::Vec3 &normal, float margin)
{
    return sphericalDistanceToNormal(radius, normal) + (margin * 2);
}

pcx::optional<Gx::RayResult> findFloor(Gx::PhysicsModel &physics, float radius, const Gx::Vec3 &base)
{
    auto r = physics.rayCast(Gx::Ray(base, Gx::Vec3(0, -1, 0)), 100);

    if(r && r->normal.dot(Gx::Vec3(0, 1, 0)) >= 0.8f && r->distance < minFloorDistance(radius, r->normal, 0.1f))
    {
        return r;
    }

    return { };
}

Gx::Vec3 alignToFloor(const Gx::Vec3 &velocity, const Gx::Vec3 &normal)
{
    return velocity.transformedNormal(Gx::Quaternion::rotationBetween(Gx::Vec3(0, 1, 0), normal).matrix());
}

Gx::Vec3 transformVelocity(const Gx::Vec3 &v, const Gx::Vec3 &n)
{
    if(fabs(n.dot(Gx::Vec3(0, 1, 0)) - 1) < 0.0001f)
    {
        return v;
    }

    auto r = v.reflected(n).normalized();
    auto p = r.perpendicular(n);

    return p.normalized() * v.length();
}

Gx::Vec3 slopeCorrection(const Gx::Vec3 &separation)
{
    auto d = separation.normalized().dot(Gx::Vec3(0, 1, 0));
    if(d >= 0 && d < 0.8f)
    {
        return transformVelocity(Gx::Vec3(0, -1, 0), separation.normalized()) * separation.length();
    }

    return Gx::Vec3(0, 0, 0);
}

Gx::Vec3 separatingVector(Gx::PhysicsModel &physics, const Gx::Shape &shape, const Gx::Vec3 &position)
{
    auto result = position;
    auto to = Gx::Matrix::translation(result);

    auto r = physics.broadphase(shape.aabb(to));

    if(!r.empty())
    {
        int it = 0;
        bool loop = true;

        while(it < 5 && loop)
        {
            loop = false;

            for(auto &b: r)
            {
                auto r = physics.convexIntersection(shape, to, b->shape(), b->matrix());

                if(r)
                {
                    result += r->separatingVector;
                    to = Gx::Matrix::translation(result);

                    loop = true;
                }
            }

            ++it;
        }
    }

    return result - position;
}

float lockedY(const Gx::RayResult &r, float radius, float height)
{
    return r.point.y + minFloorDistance(radius, r.normal, 0.05f) + ((height / 2) - radius);
}

bool lockToFloor(Gx::PhysicsModel &physics, Gx::Vec3 &pos, float radius, float height)
{
    Gx::Vec3 base(pos.x, pos.y - ((height / 2) - radius), pos.z);
    auto r = findFloor(physics, radius, base);

    if(r)
    {
        pos.y = lockedY(*r, radius, height);
        return true;
    }

    return false;
}

std::pair<Gx::Vec3, bool> moveImp(Gx::PhysicsModel &physics, const Gx::CapsuleShape &shape, const Gx::Vec3 &pos, const Gx::Vec3 &step)
{
    bool flying = step.y > 0;
    float hh = shape.height() / 2;

    auto floor = findFloor(physics, shape.radius(), pos + step + Gx::Vec3(0, -(hh - shape.radius()), 0));

    Gx::Vec3 dv(step.x, 0, step.z);
    Gx::Vec3 mv = floor && !flying ? alignToFloor(dv, floor->normal) : dv;

    mv.y = step.y;

    auto sep = separatingVector(physics, shape, pos + mv);

    if(!flying && floor)
    {
        mv += slopeCorrection(sep);
    }

    Gx::Vec3 res = pos + mv + sep;

    bool grounded = false;
    if(step.y <= 0)
    {
        grounded = lockToFloor(physics, res, shape.radius(), shape.height());
    }

    return std::make_pair(res, grounded);
}

}

Kcc::Kcc(float radius, float height, const Gx::Vec3 &position) : sh(radius, height), pos(position), gr(false)
{
}

void Kcc::move(Gx::PhysicsModel &physics, const Gx::Vec3 &step)
{
    auto m = moveImp(physics, sh, pos, step);

    pos = m.first;
    gr = m.second;
}

const Gx::CapsuleShape &Kcc::shape() const
{
    return sh;
}

Gx::Vec3 Kcc::position() const
{
    return pos;
}

bool Kcc::grounded() const
{
    return gr;
}
