#include "GameState.h"

#include "application/AppParams.h"
#include "application/FrameParams.h"
#include "application/Events.h"
#include "application/ResourcePath.h"

#include "graphics/Graphics.h"

#include "scene/SceneParams.h"

#include "debug/DebugLines.h"
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

GameState::GameState(Graphics &graphics)
{
    DebugText::init(graphics);
    model.load(graphics, scene, physics, resourcePath("assets/map.dat"));
}

GameState::~GameState()
{
    DebugText::release();
}

bool GameState::update(AppParams &app, Events &events, float delta)
{
    DebugLines::clear();
    DebugText::clear();

    cam.update(events, app.size, delta);
    physics.update(delta);

    FrameParams params;
    params.camera = cam;

    model.update(params, events, physics, delta);

    return true;
}

void GameState::render(Graphics &graphics, float blend)
{
    graphics.device.clear({ 0.2f, 0.25f, 0.3f }, 1.0f);

    auto pos = cam.transform().position();

    SceneParams params;

    params.view = cam.viewMatrix(blend);
    params.proj = Gx::Matrix::perspective(M_PI * 0.25f, graphics.size.width / graphics.size.height, { 0.1f, 100.0f });
    params.light = Gx::Vec3(pos.x, 10, pos.z - 5);

    model.prepareScene(params, blend);

    scene.render(RenderPass::Normal, graphics, params);

    DebugLines::render(graphics, scene, params);
    DebugText::render(graphics);
}
