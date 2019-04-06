#include "GameState.h"

#include "application/AppParams.h"
#include "application/FrameParams.h"
#include "application/Events.h"
#include "application/ResourcePath.h"

#include "graphics/Graphics.h"

#include "scene/SceneParams.h"

#include "entities/TestShape.h"
#include "entities/pc/Pc.h"

#include "debug/DebugRender.h"

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

GameState::GameState(Events &events, Graphics &graphics) : graphics(graphics), scene(model, graphics), pc(nullptr), shapes(0), time(0), drawPhysics(false), drawSkeleton(false), hasClosed(false)
{
    cx.connect(events.keyDown, this, &keyPressed);

    model.load(graphics, scene, physics, light, resourcePath("assets/map.dat"));
    model.addEntity(pc = new Pc(events, graphics, scene));
}

bool GameState::update(AppParams &app, Events &events, float delta)
{
    DebugPoints::clear();
    DebugLines::clear();

    if(drawPhysics)
    {
        DebugLines::addPhysics(physics, { 0, 1, 1 });
    }

    cam.update(events, app.size, delta);
    physics.update(delta);

    FrameParams params;
    params.camera = cam;

    model.update(params, events, physics, delta);

    if(shapes < 10)
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

Gx::Matrix computeDepthMatrix(const Camera &cam, SceneParams &params)
{
    auto l = -params.light.normalized();
    auto c = Gx::Vec3(-10, 10, -10);

    return Gx::Matrix::lookAt(c, c + l, Gx::Vec3(0, 0, 1)) * Gx::Matrix::ortho({ 50.0f, 50.0f }, { -10, 50 });
}

void GameState::render(Graphics &graphics, float blend)
{
    SceneParams params;

    params.viewMatrix = cam.viewMatrix(blend);
    params.projMatrix = Gx::Matrix::perspective(M_PI * 0.25f, graphics.size.width / graphics.size.height, { 0.1f, 100.0f });

    params.camera = cam.transform(blend);

    params.drawPhysics = drawPhysics;
    params.drawSkeleton = drawSkeleton;

    params.light = light;
    params.environmentDepthMatrix = computeDepthMatrix(cam, params);

    model.prepareScene(params, blend);

    scene.render(graphics, params);

    DebugPoints::render(graphics, params);
    DebugLines::render(graphics, params);
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

    if(key == 'I')
    {
        drawSkeleton = !drawSkeleton;
    }

    if(key == VK_ESCAPE)
    {
        hasClosed = true;
    }

    if(key == 'T') model.test();
}
