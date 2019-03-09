#include "Scene.h"

#include "scene/SceneParams.h"
#include "scene/nodes/SceneNode.h"

#include "debug/DebugRender.h"

#include <GxMaths/GxRect.h>

#include <GxGraphics/GxVertexDeclaration.h>
#include <GxGraphics/GxShader.h>
#include <GxGraphics/GxTexture.h>
#include <GxGraphics/GxDepthStencilSurface.h>
#include <GxGraphics/GxRenderContext.h>

namespace
{

Gx::Size depthSize = { 512, 512 };

}

Scene::Scene(Graphics &graphics)
{
    depthTex = graphics.resources.add(new Gx::Texture(graphics.device, { depthSize, 1, Gx::Graphics::Usage::Flag::RenderTarget, Gx::Graphics::Format::X8R8G8B8, Gx::Graphics::Pool::Default }));
    depthStencil = graphics.resources.add(new Gx::DepthStencilSurface(graphics.device, { depthSize, Gx::DepthStencilSurface::Format::D24S8 }));
}

void Scene::render(Graphics &graphics, SceneParams &params)
{
    Gx::RenderContext old(graphics.device);
    Gx::RenderContext depth(*depthTex, *depthStencil);

    depth.apply(graphics.device);
    graphics.device.clear({ 0, 0, 0 }, 1.0f);

    render(RenderPass::Depth, graphics, params);

    old.apply(graphics.device);
    graphics.device.clear({ 0.4f, 0.6f, 0.8f }, 1.0f);

    render(RenderPass::Normal, graphics, params);

    DebugRender::drawScreenTexture(graphics, { { 5, 5, }, { 256, 256 } }, *depthTex);
}

void Scene::render(RenderPass pass, Graphics &graphics, SceneParams &params)
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

void Scene::beginType(RenderPass pass, RenderType type, Graphics &graphics, SceneParams &params)
{
    if(type == RenderType::Color)
    {
        if(pass == RenderPass::Normal)
        {
            graphics.device.setVertexDeclaration(*graphics.colorVertexDec);
            graphics.setVertexShader(*graphics.colorVertexShader);

            graphics.currentVertexShader()->setMatrix(graphics.device, "world", Gx::Matrix::identity());
            graphics.currentVertexShader()->setMatrix(graphics.device, "viewproj", params.view * params.proj);

            graphics.device.setZBufferEnable(false);
            graphics.device.setZWriteEnable(false);
        }
    }
    else if(type == RenderType::Mesh)
    {
        graphics.device.setVertexDeclaration(*graphics.meshVertexDec);

        if(pass == RenderPass::Normal)
        {
            graphics.setVertexShader(*graphics.meshVertexShader);

            graphics.currentVertexShader()->setMatrix(graphics.device, "viewproj", params.view * params.proj);
            graphics.currentVertexShader()->setVector(graphics.device, "light", params.light);
        }
        else if(pass == RenderPass::Depth)
        {
            graphics.setVertexShader(*graphics.depthVertexShader);
            graphics.currentVertexShader()->setMatrix(graphics.device, "viewproj", params.depth);

            graphics.setPixelShader(*graphics.depthPixelShader);
        }
    }
}

void Scene::endType(Graphics &graphics)
{
    graphics.device.setZBufferEnable(true);
    graphics.device.setZWriteEnable(true);

    graphics.setPixelShader();
}
