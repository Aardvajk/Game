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

GameState::GameState(Graphics &graphics) : pc(nullptr)
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

    cam.update(events, delta);
    pc->update(events, physics, cam.transform(), delta);

    return true;
}

void GameState::render(Graphics &graphics, float blend)
{
    SceneParams params;

    params.view = cam.viewMatrix(blend);
    params.proj = Gx::Matrix::perspective(M_PI * 0.25f, 1024.0f / 768.0f, { 0.1f, 100.0f });
    params.camera = cam.transform();

    pc->prepareScene(params, blend);

    scene.render(graphics, params);

    DebugRender::render(graphics, params);
    DebugText::render(graphics);
}
