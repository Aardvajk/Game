#ifndef SCENEPARAMS_H
#define SCENEPARAMS_H

#include "scene/RenderTypes.h"

#include <GxMaths/GxVector.h>
#include <GxMaths/GxMatrix.h>

class SceneParams
{
public:
    SceneParams() = default;

    Gx::Matrix depthMatrix(RenderPass pass) const;

    Gx::Matrix viewMatrix;
    Gx::Matrix projMatrix;

    Gx::Matrix objectDepthMatrix;
    Gx::Matrix environmentDepthMatrix;

    Gx::Vec3 light;
};

#endif // SCENEPARAMS_H
