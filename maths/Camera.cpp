#include "Camera.h"

#include "application/Events.h"

Camera::Camera() : tr(Gx::Vec3(0, 3, -10), Gx::Vec2(0, 0.3f))
{
}

void Camera::update(Events &events, float delta)
{
    tr.store();

    auto pos = tr.value().position();
    auto ang = tr.value().angle();

    Gx::Vec3 forw, right;
    tr.value().flatVectors(forw, right);

    float speed = 10.0f * delta;

    if(events.isKeyDown(VK_RBUTTON))
    {
        if(events.isKeyDown('A')) pos -= right * speed;
        if(events.isKeyDown('D')) pos += right * speed;

        if(events.isKeyDown('W')) pos += forw * speed;
        if(events.isKeyDown('S')) pos -= forw * speed;

        if(events.isKeyDown('R')) pos.y += speed;
        if(events.isKeyDown('F')) pos.y -= speed;

        if(events.isKeyDown(VK_RBUTTON))
        {
            auto diff = events.rawMouseDelta();
            if(diff.length())
            {
                float turn = 0.6f;

                Gx::Vec2 dims = Gx::Vec2(1024.0f, 768.0f) * 0.5f;
                ang += Gx::Vec2((diff.x / dims.x) * turn, (diff.y / dims.y) * turn);
            }
        }

        tr.set(Gx::Transform(pos, ang));
    }
}

Gx::Transform Camera::transform() const
{
    return tr.value();
}

Gx::Matrix Camera::viewMatrix(float blend) const
{
    auto look = Gx::Vec3(0, 0, 1).transformedNormal(tr.value(blend).rotation().matrix());
    auto up = Gx::Vec3(0, 1, 0).transformedNormal(tr.value(blend).rotation().matrix());

    auto pos = tr.value(blend).position();

    return Gx::Matrix::lookAt(pos, pos + look, up);
}