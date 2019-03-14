#include "GameState.h"

#include "application/AppParams.h"
#include "application/FrameParams.h"
#include "application/Events.h"
#include "application/ResourcePath.h"

#include "graphics/Graphics.h"

#include "scene/SceneParams.h"

#include "entities/TestBox.h"

#include "debug/DebugRender.h"

#include <GxCore/GxDebug.h>

#include <GxMaths/GxColor.h>
#include <GxMaths/GxVector.h>
#include <GxMaths/GxMatrix.h>
#include <GxMaths/GxRange.h>

#include <GxGraphics/GxBufferStream.h>

GameState::GameState(Events &events, Graphics &graphics) : graphics(graphics), scene(graphics), drawPhysics(false)
{
    cx.connect(events.keyDown, this, &keyPressed);

    model.load(graphics, scene, physics, resourcePath("assets/map.dat"));
}

bool GameState::update(AppParams &app, Events &events, float delta)
{
    DebugPoints::clear();
    DebugLines::clear();

    if(drawPhysics)
    {
        DebugLines::addPhysics(physics);
    }

    cam.update(events, app.size, delta);
    physics.update(delta);

    FrameParams params;
    params.camera = cam;

    model.update(params, events, physics, delta);

    return true;
}

void GameState::render(Graphics &graphics, float blend)
{
    auto pos = cam.transform().position();

    SceneParams params;

    params.viewMatrix = cam.viewMatrix(blend);
    params.projMatrix = Gx::Matrix::perspective(M_PI * 0.25f, graphics.size.width / graphics.size.height, { 0.1f, 100.0f });
    params.mainDepthMatrix = Gx::Matrix::lookAt(Gx::Vec3(0, 10, 0), Gx::Vec3(3, 1, 3), Gx::Vec3(0, 0, 1)) * Gx::Matrix::ortho({ 60.0f, 60.0f }, { -10, 30 });

    params.light = Gx::Vec3(-1.2f, 1, -0.8f);

    model.prepareScene(params, blend);
    scene.render(graphics, params);

    DebugPoints::render(graphics, params);
    DebugLines::render(graphics, params);
}

void GameState::keyPressed(int key)
{
    if(key == 'P')
    {
        auto r = [](){ return 1.0f + ((std::rand() % 16) / 20.0f); };

        auto dims = Gx::Vec3(r(), r(), r());
        auto pos = Gx::Vec3((std::rand() % 30) - 15, 10, (std::rand() % 20) - 10);

        model.addEntity(new TestBox(graphics, scene, physics, dims, pos));
    }

    if(key == 'O')
    {
        drawPhysics = !drawPhysics;
    }
}
