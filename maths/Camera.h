#ifndef CAMERA_H
#define CAMERA_H

#include <GxMaths/GxMatrix.h>
#include <GxMaths/GxTransform.h>
#include <GxMaths/GxSize.h>

class Events;

class Camera
{
public:
    Camera();

    void update(Events &events, const Gx::SizeF &size, float delta);

    Gx::Transform transform() const;
    Gx::Matrix viewMatrix(float blend) const;

private:
    Gx::BlendTransform tr;
};

#endif // CAMERA_H
