#ifndef SCENEPARAMS_H
#define SCENEPARAMS_H

#include "scene/RenderTypes.h"

#include <GxMaths/GxVector.h>
#include <GxMaths/GxMatrix.h>
#include <GxMaths/GxTransform.h>

class SceneParams
{
public:
    SceneParams() = default;

    Gx::Matrix depthMatrix(RenderPass pass) const;

    Gx::Matrix viewMatrix;
    Gx::Matrix projMatrix;

    Gx::Matrix objectDepthMatrix;
    Gx::Matrix environmentDepthMatrix;

    Gx::Transform camera;
    Gx::Vec3 light;

    bool drawPhysics;
    bool drawSkeleton;

    float test;
};

#endif // SCENEPARAMS_H
