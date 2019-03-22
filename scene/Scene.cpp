#include "Scene.h"

#include "models/Model.h"

#include "scene/SceneParams.h"
#include "scene/nodes/SceneNode.h"

#include "debug/DebugRender.h"

#include <GxMaths/GxRect.h>

#include <GxGraphics/GxVertexDeclaration.h>
#include <GxGraphics/GxShader.h>
#include <GxGraphics/GxTexture.h>
#include <GxGraphics/GxDepthStencilSurface.h>
#include <GxGraphics/GxRenderContext.h>

#include <pcx/optional.h>

namespace
{

Gx::Size objectDepthSize = { 256, 256 };
Gx::Size environmentDepthSize = { 2048, 2048 };

}

Scene::Scene(Model &model, Graphics &graphics) : model(model)
{
    objectDepthTex = graphics.resources.add(new Gx::Texture(graphics.device, { objectDepthSize, 1, Gx::Graphics::Usage::Flag::RenderTarget, Gx::Graphics::Format::X8R8G8B8, Gx::Graphics::Pool::Default }));
    objectDepthStencil = graphics.resources.add(new Gx::DepthStencilSurface(graphics.device, { objectDepthSize, Gx::DepthStencilSurface::Format::D24S8 }));

    environmentDepthTex = graphics.resources.add(new Gx::Texture(graphics.device, { environmentDepthSize, 1, Gx::Graphics::Usage::Flag::RenderTarget, Gx::Graphics::Format::X8R8G8B8, Gx::Graphics::Pool::Default }));
    environmentDepthStencil = graphics.resources.add(new Gx::DepthStencilSurface(graphics.device, { environmentDepthSize, Gx::DepthStencilSurface::Format::D24S8 }));
}

Scene::~Scene() = default;

void Scene::render(Graphics &graphics, SceneParams &params)
{
    Gx::RenderContext old(graphics.device);
    Gx::RenderContext playerDepth(*objectDepthTex, *objectDepthStencil);
    Gx::RenderContext environmentDepth(*environmentDepthTex, *environmentDepthStencil);

    playerDepth.apply(graphics.device);
    graphics.device.clear({ 0, 0, 0 }, 1.0f);

    render(RenderPass::ObjectDepth, graphics, params);

    environmentDepth.apply(graphics.device);
    graphics.device.clear({ 0, 0, 0 }, 1.0f);

    render(RenderPass::EnvironmentDepth, graphics, params);

    old.apply(graphics.device);
    graphics.device.clear({ 0.4f, 0.6f, 0.8f }, 1.0f);

    render(RenderPass::Normal, graphics, params);

    DebugRender::drawScreenTexture(graphics, { { 5, 5 }, { 128, 128 } }, *environmentDepthTex, graphics.unpackPixelShader.get());
    DebugRender::drawScreenTexture(graphics, { { 138, 5 }, { 64, 64 } }, *objectDepthTex, graphics.unpackPixelShader.get());
}

void Scene::render(RenderPass pass, Graphics &graphics, SceneParams &params)
{
    pcx::optional<RenderType> currType;
    pcx::optional<RenderKey> currKey;

    for(auto i: ns)
    {
        if(i->pass(pass))
        {
            auto type = i->type();
            auto key = i->key();

            if(currType != type || currKey != key)
            {
                endType(graphics);

                currType = type;
                currKey = key;

                beginType(pass, type, key, graphics, params);
            }

            if(currType)
            {
                i->render(pass, graphics, params);
            }
        }
    }

    endType(graphics);
}

void Scene::beginType(RenderPass pass, RenderType type, const RenderKey &key, Graphics &graphics, SceneParams &params)
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
    else if(type == RenderType::Object || type == RenderType::Environment)
    {
        graphics.device.setVertexDeclaration(*graphics.meshVertexDec);

        if(pass == RenderPass::Normal)
        {
            graphics.setVertexShader(*graphics.meshVertexShader);

            graphics.currentVertexShader()->setMatrix(graphics.device, "viewproj", params.viewMatrix * params.projMatrix);
            graphics.currentVertexShader()->setVector(graphics.device, "light", params.light);

            graphics.setPixelShader(model.pixelShader(key.features()));

            if(key.shadows())
            {
                graphics.currentPixelShader()->setMatrix(graphics.device, "objectWorldviewproj", params.objectDepthMatrix);
                graphics.currentPixelShader()->setMatrix(graphics.device, "environmentWorldviewproj", params.environmentDepthMatrix);
                graphics.currentPixelShader()->setFloat(graphics.device, "objectTexel", 1.0f / static_cast<float>(objectDepthTex->size().width));
                graphics.currentPixelShader()->setFloat(graphics.device, "environmentTexel", 1.0f / static_cast<float>(environmentDepthTex->size().width));

                graphics.device.setTexture(0, *objectDepthTex);
                graphics.device.setTextureFilter(0, Gx::Graphics::Filter::None);

                graphics.device.setTexture(1, *environmentDepthTex);
                graphics.device.setTextureFilter(1, Gx::Graphics::Filter::None);
            }

            if(key.diffuse())
            {
                graphics.device.setTexture(2, *key.diffuse());
            }
        }
        else if(pass == RenderPass::ObjectDepth || pass == RenderPass::EnvironmentDepth)
        {
            graphics.setVertexShader(*graphics.depthVertexShader);
            graphics.currentVertexShader()->setMatrix(graphics.device, "viewproj", params.depthMatrix(pass));

            graphics.setPixelShader(*graphics.depthPixelShader);
        }
    }
    else if(type == RenderType::SkyBox)
    {
        if(pass == RenderPass::Normal)
        {
            graphics.device.setVertexDeclaration(*graphics.positionVertexDec);

            graphics.setVertexShader(*graphics.skyBoxVertexShader);
            graphics.currentVertexShader()->setMatrix(graphics.device, "worldviewproj", Gx::Matrix::translation(params.camera.position()) * params.viewMatrix * params.projMatrix);

            graphics.setPixelShader(*graphics.skyBoxPixelShader);
        }
    }
}

void Scene::endType(Graphics &graphics)
{
    graphics.device.setZBufferEnable(true);
    graphics.device.setZWriteEnable(true);

    graphics.setPixelShader();

    graphics.device.setTexture(0);
    graphics.device.setTextureFilter(0, Gx::Graphics::Filter::Linear);

    graphics.device.setTexture(1);
    graphics.device.setTextureFilter(1, Gx::Graphics::Filter::Linear);

    graphics.device.setTexture(2);
}
