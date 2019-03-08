#include "Scene.h"

#include "graphics/Graphics.h"

#include "scene/SceneParams.h"
#include "scene/nodes/SceneNode.h"

#include <GxGraphics/GxVertexDeclaration.h>
#include <GxGraphics/GxShader.h>

Scene::Scene() = default;

void Scene::render(RenderPass pass, Graphics &graphics, const SceneParams &params)
{
    RenderType curr = RenderType::Null;

    for(auto i: ns)
    {
        if(i->pass(pass))
        {
            RenderType type = i->type();

            if(curr != type)
            {
                endType(graphics);

                curr = type;
                beginType(pass, type, graphics, params);
            }

            if(curr != RenderType::Null)
            {
                i->render(pass, graphics, params);
            }
        }
    }

    endType(graphics);
}

void Scene::beginType(RenderPass pass, RenderType type, Graphics &graphics, const SceneParams &params)
{
    if(type == RenderType::Color)
    {
        graphics.device.setVertexDeclaration(*graphics.colorVertexDec);
        graphics.device.setVertexShader(*graphics.colorShader);

        graphics.colorShader->setMatrix(graphics.device, "world", Gx::Matrix::identity());
        graphics.colorShader->setMatrix(graphics.device, "viewproj", params.view * params.proj);

        graphics.device.setZBufferEnable(false);
        graphics.device.setZWriteEnable(false);
    }
    else if(type == RenderType::Mesh)
    {
        graphics.device.setVertexDeclaration(*graphics.meshVertexDec);
        graphics.device.setVertexShader(*graphics.meshShader);

        graphics.meshShader->setMatrix(graphics.device, "viewproj", params.view * params.proj);
        graphics.meshShader->setVector(graphics.device, "light", params.light);
    }
}

void Scene::endType(Graphics &graphics)
{
    graphics.device.setZBufferEnable(true);
    graphics.device.setZWriteEnable(true);
}
