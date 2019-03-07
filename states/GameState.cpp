#include "GameState.h"

#include "application/AppParams.h"
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

#include "entities/pc/Pc.h"

GameState::GameState(Graphics &graphics) : pc(nullptr)
{
    DebugText::init(graphics);
    model.load(graphics, scene, physics, resourcePath("assets/map.dat"));

    pc = new Pc(graphics, scene);
}

GameState::~GameState()
{
    DebugText::release();
}

bool GameState::update(AppParams &params, Events &events, float delta)
{
    DebugLines::clear();
    DebugText::clear();

    cam.update(events, params.size, delta);
    pc->update(events, physics, cam.transform(), delta);

    return true;
}

void GameState::render(Graphics &graphics, float blend)
{
    SceneParams params;

    params.view = cam.viewMatrix(blend);
    params.proj = Gx::Matrix::perspective(M_PI * 0.25f, graphics.size.width / graphics.size.height, { 0.1f, 100.0f });
    params.camera = cam.transform();

    pc->prepareScene(params, blend);

    scene.render(graphics, params);

    DebugLines::render(graphics, params);
    DebugText::render(graphics);
}
