#include "GameState.h"

#include "application/AppParams.h"
#include "application/FrameParams.h"
#include "application/Events.h"
#include "application/ResourcePath.h"

#include "graphics/Graphics.h"

#include "scene/SceneParams.h"

#include "entities/TestShape.h"

#include "debug/DebugRender.h"
#include "debug/DebugText.h"

#include <GxCore/GxDebug.h>

#include <GxMaths/GxColor.h>
#include <GxMaths/GxVector.h>
#include <GxMaths/GxMatrix.h>
#include <GxMaths/GxRange.h>

#include <pcx/str.h>

namespace
{

void addTestShape(Model &model, Graphics &graphics, Scene &scene, Gx::PhysicsModel &physics)
{
    auto r = [](){ return 1.0f + ((std::rand() % 16) / 20.0f); };

    auto dims = Gx::Vec3(r(), r(), r());
    auto pos = Gx::Vec3((std::rand() % 30) - 15, 20, (std::rand() % 20) - 10);

    model.addEntity(new TestShape(graphics, scene, physics, dims, pos));
}

}

GameState::GameState(Events &events, Graphics &graphics) : graphics(graphics), scene(model, graphics), drawPhysics(false), hasClosed(false), shapes(0), time(0)
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

    if(shapes < 100)
    {
        time += delta;
        if(time > 0.05f)
        {
            time = 0;
            ++shapes;

            addTestShape(model, graphics, scene, physics);
        }
    }

    return !hasClosed;
}

Gx::Matrix computeDepthMatrix(const Camera &cam, const SceneParams &params)
{
    return Gx::Matrix::lookAt(Gx::Vec3(0, 10, 0), Gx::Vec3(5, 1, 5), Gx::Vec3(0, 0, 1)) * Gx::Matrix::ortho({ 60.0f, 60.0f }, { -10, 30 });
}

void GameState::render(Graphics &graphics, float blend)
{
    SceneParams params;

    params.viewMatrix = cam.viewMatrix(blend);
    params.projMatrix = Gx::Matrix::perspective(M_PI * 0.25f, graphics.size.width / graphics.size.height, { 0.1f, 100.0f });

    params.camera = cam.transform(blend);
    params.light = Gx::Vec3(-1.2f, 1, -0.8f);

    params.environmentDepthMatrix = computeDepthMatrix(cam, params);

    model.prepareScene(params, blend);
    scene.render(graphics, params);

    DebugPoints::render(graphics, params);
    DebugLines::render(graphics, params);

    auto s = pcx::str("Shapes: ", shapes);
    DebugText::draw(2, 768 - (DebugText::height() + 2), s);
}

void GameState::keyPressed(int key)
{
    if(key == 'P')
    {
        addTestShape(model, graphics, scene, physics);
    }

    if(key == 'O')
    {
        drawPhysics = !drawPhysics;
    }

    if(key == VK_ESCAPE)
    {
        hasClosed = true;
    }
}
