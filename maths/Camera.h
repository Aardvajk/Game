#ifndef CAMERA_H
#define CAMERA_H

#include <GxMaths/GxMatrix.h>
#include <GxMaths/GxTransform.h>

class Events;

namespace Gx
{

class SizeF;

}

class Camera
{
public:
    Camera();

    void update(Events &events, const Gx::SizeF &size, float delta);

    Gx::Transform transform() const;
    Gx::Transform transform(float blend) const;

    Gx::Matrix viewMatrix(float blend) const;

private:
    Gx::BlendTransform tr;
};

#endif // CAMERA_H
