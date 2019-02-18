#include "GameState.h"

#include "graphics/Graphics.h"

#include <GxMaths/GxColor.h>
#include <GxMaths/GxVector.h>
#include <GxMaths/GxMatrix.h>
#include <GxMaths/GxRange.h>

#include "debug/DebugMesh.h"

#include "graphics/vertices/MeshVertex.h"

GameState::GameState(Graphics &graphics) : graphics(graphics), cam(Gx::Vec3(0, 0, -10), Gx::Vec2(0, 0))
{
    cuboid = graphics.resources.add(new Gx::VertexBuffer(graphics.device, { 25600 * sizeof(MeshVertex), Gx::Graphics::Usage::Flag::Dynamic, Gx::Graphics::Pool::Default }));
    capsule = graphics.resources.add(new Gx::VertexBuffer(graphics.device, { 25600 * sizeof(MeshVertex), Gx::Graphics::Usage::Flag::Dynamic, Gx::Graphics::Pool::Default }));

    cx.connect(graphics.deviceReset, this, &deviceReset);

    deviceReset();
}

bool GameState::update(float delta)
{
    auto pos = cam.position();

    Gx::Vec3 forw, right;
    cam.flatVectors(forw, right);

    float speed = 10.0f * delta;

    if(GetAsyncKeyState('A') & 0x8000) pos -= right * speed;
    if(GetAsyncKeyState('D') & 0x8000) pos += right * speed;

    if(GetAsyncKeyState('W') & 0x8000) pos += forw * speed;
    if(GetAsyncKeyState('S') & 0x8000) pos -= forw * speed;

    if(GetAsyncKeyState('R') & 0x8000) pos.y += speed;
    if(GetAsyncKeyState('F') & 0x8000) pos.y -= speed;

    POINT p;
    GetCursorPos(&p);

    Gx::Vec2 mouse;
    mouse.x = static_cast<float>(p.x);
    mouse.y = static_cast<float>(p.y);

    if(GetAsyncKeyState(VK_RBUTTON) & 0x8000)
    {
        if(prevMouse != mouse)
        {
            float turn = 0.4f;

            Gx::Vec2 dims = Gx::Vec2(1024.0f, 768.0f) * 0.5f;
            Gx::Vec2 diff = mouse - prevMouse;

            Gx::Vec2 ang = cam.angle();

            ang += Gx::Vec2((diff.x / dims.x) * turn, (diff.y / dims.y) * turn);

            cam.setAngle(ang);
        }
    }

    prevMouse = mouse;

    cam.setPosition(pos);

    return true;
}

void GameState::render(float blend)
{
    auto look = Gx::Vec3(0, 0, 1).transformedNormal(cam.rotation().matrix());
    auto up = Gx::Vec3(0, 1, 0).transformedNormal(cam.rotation().matrix());

    graphics.device.clear({ 0.2f, 0.5f, 0.8f }, 1.0f);

    graphics.device.setVertexDeclaration(*graphics.meshVertexDec);
    graphics.device.setVertexShader(*graphics.meshShader);

    graphics.meshShader->setMatrix(graphics.device, "viewproj", Gx::Matrix::lookAt(cam.position(), cam.position() + look, up) * Gx::Matrix::perspective(M_PI * 0.25f, 1024.0f / 768.0f, { 0.1f, 100.0f }));
    graphics.meshShader->setVector(graphics.device, "light", cam.position());

    graphics.meshShader->setMatrix(graphics.device, "world", Gx::Matrix::translation({ -2, 0, 0 }));
    graphics.device.renderTriangleList(*cuboid, cuboidCount);

    graphics.meshShader->setMatrix(graphics.device, "world", Gx::Matrix::translation({ 2, 0, 0 }));
    graphics.device.renderTriangleList(*capsule, capsuleCount);
}

void GameState::deviceReset()
{
    cuboidCount = debugCuboidToBuffer(*cuboid, { 2, 2, 2 }, Gx::Color(1, 0, 0));
    capsuleCount = debugCapsuleToBuffer(*capsule, 16, 16, 0.5f, 2.0f, Gx::Color(0, 1, 0));
}
