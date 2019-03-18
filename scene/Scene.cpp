#include "Scene.h"

#include "scene/SceneParams.h"
#include "scene/nodes/SceneNode.h"

#include <GxMaths/GxRect.h>

#include <GxGraphics/GxVertexDeclaration.h>
#include <GxGraphics/GxShader.h>
#include <GxGraphics/GxTexture.h>
#include <GxGraphics/GxDepthStencilSurface.h>
#include <GxGraphics/GxRenderContext.h>

namespace
{

Gx::Size playerDepthSize = { 256, 256 };
Gx::Size mainDepthSize = { 2048, 2048 };

}

Scene::Scene(Graphics &graphics)
{
    playerDepthTex = graphics.resources.add(new Gx::Texture(graphics.device, { playerDepthSize, 1, Gx::Graphics::Usage::Flag::RenderTarget, Gx::Graphics::Format::X8R8G8B8, Gx::Graphics::Pool::Default }));
    playerDepthStencil = graphics.resources.add(new Gx::DepthStencilSurface(graphics.device, { playerDepthSize, Gx::DepthStencilSurface::Format::D24S8 }));

    mainDepthTex = graphics.resources.add(new Gx::Texture(graphics.device, { mainDepthSize, 1, Gx::Graphics::Usage::Flag::RenderTarget, Gx::Graphics::Format::X8R8G8B8, Gx::Graphics::Pool::Default }));
    mainDepthStencil = graphics.resources.add(new Gx::DepthStencilSurface(graphics.device, { mainDepthSize, Gx::DepthStencilSurface::Format::D24S8 }));
}

void Scene::render(Graphics &graphics, SceneParams &params)
{
    Gx::RenderContext old(graphics.device);
    Gx::RenderContext playerDepth(*playerDepthTex, *playerDepthStencil);
    Gx::RenderContext mainDepth(*mainDepthTex, *mainDepthStencil);

    playerDepth.apply(graphics.device);
    graphics.device.clear({ 0, 0, 0 }, 1.0f);

    render(RenderPass::PlayerDepth, graphics, params);

    mainDepth.apply(graphics.device);
    graphics.device.clear({ 0, 0, 0 }, 1.0f);

    render(RenderPass::MainDepth, graphics, params);

    old.apply(graphics.device);
    graphics.device.clear({ 0.4f, 0.6f, 0.8f }, 1.0f);

    render(RenderPass::Normal, graphics, params);
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
            graphics.currentVertexShader()->setMatrix(graphics.device, "viewproj", params.viewMatrix * params.projMatrix);

            graphics.device.setZBufferEnable(false);
            graphics.device.setZWriteEnable(false);
        }
    }
    else if(type == RenderType::Player)
    {
        graphics.device.setVertexDeclaration(*graphics.meshVertexDec);

        if(pass == RenderPass::Normal)
        {
            graphics.setVertexShader(*graphics.meshVertexShader);

            graphics.currentVertexShader()->setMatrix(graphics.device, "viewproj", params.viewMatrix * params.projMatrix);
            graphics.currentVertexShader()->setVector(graphics.device, "light", params.light);
        }
        else if(pass == RenderPass::PlayerDepth)
        {
            graphics.setVertexShader(*graphics.depthVertexShader);
            graphics.currentVertexShader()->setMatrix(graphics.device, "viewproj", params.playerDepthMatrix);

            graphics.setPixelShader(*graphics.depthPixelShader);
        }
    }
    else if(type == RenderType::Surface)
    {
        graphics.device.setVertexDeclaration(*graphics.meshVertexDec);

        if(pass == RenderPass::Normal)
        {
            graphics.setVertexShader(*graphics.surfaceVertexShader);

            graphics.currentVertexShader()->setMatrix(graphics.device, "viewproj", params.viewMatrix * params.projMatrix);
            graphics.currentVertexShader()->setVector(graphics.device, "light", params.light);

            graphics.setPixelShader(*graphics.shadowPixelShader);

            graphics.currentPixelShader()->setMatrix(graphics.device, "pcWorldviewproj", params.playerDepthMatrix);
            graphics.currentPixelShader()->setMatrix(graphics.device, "mainWorldviewproj", params.mainDepthMatrix);
            graphics.currentPixelShader()->setFloat(graphics.device, "pcTexel", 1.0f / static_cast<float>(playerDepthTex->size().width));
            graphics.currentPixelShader()->setFloat(graphics.device, "mainTexel", 1.0f / static_cast<float>(mainDepthTex->size().width));

            graphics.device.setTexture(0, *playerDepthTex);
            graphics.device.setTextureFilter(0, Gx::Texture::Filter::None);

            graphics.device.setTexture(1, *mainDepthTex);
            graphics.device.setTextureFilter(1, Gx::Texture::Filter::None);
        }
        else if(pass == RenderPass::MainDepth)
        {
            graphics.setVertexShader(*graphics.depthVertexShader);
            graphics.currentVertexShader()->setMatrix(graphics.device, "viewproj", params.mainDepthMatrix);

            graphics.setPixelShader(*graphics.depthPixelShader);
        }
    }
}

void Scene::endType(Graphics &graphics)
{
    graphics.device.setZBufferEnable(true);
    graphics.device.setZWriteEnable(true);

    graphics.setPixelShader();

    graphics.device.setTexture(0);
    graphics.device.setTextureFilter(0, Gx::Texture::Filter::Linear);

    graphics.device.setTexture(1);
    graphics.device.setTextureFilter(1, Gx::Texture::Filter::Linear);

    graphics.device.setTexture(2);
}
