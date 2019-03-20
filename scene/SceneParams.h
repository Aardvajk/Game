#ifndef SCENEPARAMS_H
#define SCENEPARAMS_H

#include <GxMaths/GxVector.h>
#include <GxMaths/GxMatrix.h>

class SceneParams
{
public:
    SceneParams() = default;

    Gx::Matrix viewMatrix;
    Gx::Matrix projMatrix;

    Gx::Matrix objectDepthMatrix;
    Gx::Matrix mainDepthMatrix;

    Gx::Vec3 light;
};

#endif // SCENEPARAMS_H
