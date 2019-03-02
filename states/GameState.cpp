#include "GameState.h"

#include "graphics/Graphics.h"

#include "scene/SceneParams.h"

#include "debug/DebugRender.h"

#include <GxCore/GxDebug.h>

#include <GxMaths/GxColor.h>
#include <GxMaths/GxVector.h>
#include <GxMaths/GxMatrix.h>
#include <GxMaths/GxRange.h>

#include <GxGraphics/GxBufferStream.h>

#include "debug/DebugMesh.h"

#include "graphics/vertices/MeshVertex.h"

#include "entities/pc/Pc.h"

#include <pcx/datastream.h>

GameState::GameState(Graphics &graphics) : graphics(graphics), cam(Gx::Vec3(0, 3, -10), Gx::Vec2(0, 0.3f)), pc(nullptr)
{
    model.load(graphics, scene, physics, "C:/Users/aardv/Desktop/map.dat");

    pc = new Pc(graphics, scene);
}

bool GameState::update(float delta)
{
    auto pos = cam.position();

    Gx::Vec3 forw, right;
    cam.flatVectors(forw, right);

    float speed = 10.0f * delta;

    POINT p;
    GetCursorPos(&p);

    Gx::Vec2 mouse;
    mouse.x = static_cast<float>(p.x);
    mouse.y = static_cast<float>(p.y);

    if(GetAsyncKeyState(VK_RBUTTON) & 0x8000)
    {
        if(GetAsyncKeyState('A') & 0x8000) pos -= right * speed;
        if(GetAsyncKeyState('D') & 0x8000) pos += right * speed;

        if(GetAsyncKeyState('W') & 0x8000) pos += forw * speed;
        if(GetAsyncKeyState('S') & 0x8000) pos -= forw * speed;

        if(GetAsyncKeyState('R') & 0x8000) pos.y += speed;
        if(GetAsyncKeyState('F') & 0x8000) pos.y -= speed;

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

        cam.setPosition(pos);
    }

    prevMouse = mouse;

    DebugRender::clear();

    pc->update(physics, cam, delta);

    return true;
}

void GameState::render(float blend)
{
    auto look = Gx::Vec3(0, 0, 1).transformedNormal(cam.rotation().matrix());
    auto up = Gx::Vec3(0, 1, 0).transformedNormal(cam.rotation().matrix());

    SceneParams params;

    params.view = Gx::Matrix::lookAt(cam.position(), cam.position() + look, up);
    params.proj = Gx::Matrix::perspective(M_PI * 0.25f, 1024.0f / 768.0f, { 0.1f, 100.0f });
    params.camera = cam;

    scene.render(graphics, params);

    DebugRender::render(graphics, params);
}
