#include "Scene.h"

#include "graphics/Graphics.h"

#include "scene/SceneParams.h"
#include "scene/nodes/SceneNode.h"

#include <GxGraphics/GxVertexDeclaration.h>
#include <GxGraphics/GxShader.h>

void Scene::render(Graphics &graphics, const SceneParams &params)
{
    graphics.device.clear({ 0.2f, 0.25f, 0.3f }, 1.0f);

    beginType(graphics, params);

    for(auto n: ns)
    {
        n->render(graphics);
    }

    endType(graphics);
}

void Scene::beginType(Graphics &graphics, const SceneParams &params)
{
    graphics.device.setVertexDeclaration(*graphics.meshVertexDec);
    graphics.device.setVertexShader(*graphics.meshShader);

    graphics.meshShader->setMatrix(graphics.device, "viewproj", params.view * params.proj);
    graphics.meshShader->setVector(graphics.device, "light", params.camera.position());
}

void Scene::endType(Graphics &graphics)
{
}
