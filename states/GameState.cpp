#include "GameState.h"

#include "application/Events.h"

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
#include "debug/DebugText.h"

#include "graphics/vertices/MeshVertex.h"

#include "entities/pc/Pc.h"

GameState::GameState(Graphics &graphics) : cam(Gx::Vec3(0, 3, -10), Gx::Vec2(0, 0.3f)), pc(nullptr)
{
    DebugText::init(graphics);
    model.load(graphics, scene, physics, "C:/Projects/Game/Game/map.dat");

    pc = new Pc(graphics, scene);
}

GameState::~GameState()
{
    DebugText::release();
}

bool GameState::update(Events &events, float delta)
{
    DebugRender::clear();
    DebugText::clear();

    auto pos = cam.position();

    Gx::Vec3 forw, right;
    cam.flatVectors(forw, right);

    float speed = 10.0f * delta;

    if(events.isKeyDown(VK_RBUTTON))
    {
        if(events.isKeyDown('A')) pos -= right * speed;
        if(events.isKeyDown('D')) pos += right * speed;

        if(events.isKeyDown('W')) pos += forw * speed;
        if(events.isKeyDown('S')) pos -= forw * speed;

        if(events.isKeyDown('R')) pos.y += speed;
        if(events.isKeyDown('F')) pos.y -= speed;

        if(GetAsyncKeyState(VK_RBUTTON) & 0x8000)
        {
            auto diff = events.rawMouseDelta();
            if(diff.length())
            {
                float turn = 0.6f;

                Gx::Vec2 dims = Gx::Vec2(1024.0f, 768.0f) * 0.5f;
                Gx::Vec2 ang = cam.angle();

                ang += Gx::Vec2((diff.x / dims.x) * turn, (diff.y / dims.y) * turn);

                cam.setAngle(ang);
            }
        }

        cam.setPosition(pos);
    }

    pc->update(events, physics, cam, delta);

    return true;
}

void GameState::render(Graphics &graphics, float blend)
{
    auto look = Gx::Vec3(0, 0, 1).transformedNormal(cam.rotation().matrix());
    auto up = Gx::Vec3(0, 1, 0).transformedNormal(cam.rotation().matrix());

    SceneParams params;

    params.view = Gx::Matrix::lookAt(cam.position(), cam.position() + look, up);
    params.proj = Gx::Matrix::perspective(M_PI * 0.25f, 1024.0f / 768.0f, { 0.1f, 100.0f });
    params.camera = cam;

    pc->prepareScene(params, blend);

    scene.render(graphics, params);

    DebugRender::render(graphics, params);
    DebugText::render(graphics);
}
