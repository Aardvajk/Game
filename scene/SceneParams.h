#ifndef SCENEPARAMS_H
#define SCENEPARAMS_H

#include <GxMaths/GxMatrix.h>
#include <GxMaths/GxTransform.h>

class SceneParams
{
public:
    SceneParams() = default;

    Gx::Matrix view;
    Gx::Matrix proj;

    Gx::Transform camera;
};

#endif // SCENEPARAMS_H
