#ifndef SCENEPARAMS_H
#define SCENEPARAMS_H

#include <GxMaths/GxVector.h>
#include <GxMaths/GxMatrix.h>

class SceneParams
{
public:
    SceneParams() = default;

    Gx::Matrix view;
    Gx::Matrix proj;

    Gx::Vec3 light;
};

#endif // SCENEPARAMS_H
