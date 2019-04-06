#include "Graphics.h"

#include "application/ResourcePath.h"

#include "graphics/VertexBuffer.h"

#include "graphics/vertices/ColorVertex.h"
#include "graphics/vertices/MeshVertex.h"
#include "graphics/vertices/TextureVertex.h"
#include "graphics/vertices/PositionVertex.h"
#include "graphics/vertices/RigVertex.h"

#include <GxGraphics/GxGraphicsTypes.h>
#include <GxGraphics/GxVertexDeclaration.h>
#include <GxGraphics/GxShader.h>
#include <GxGraphics/GxTexture.h>

#include <fstream>

namespace
{

class Cache
{
public:
    Cache() : vs(nullptr), ps(nullptr) { }

    Gx::VertexShader *vs;
    Gx::PixelShader *ps;
};

}

Graphics::Graphics(HWND hw, const Gx::DisplaySettings &settings) : device(hw, settings), size(settings.size)
{
    cache.alloc<Cache>();

    colorVertexDec = resources.add(new Gx::VertexDeclaration(device, ColorVertex::declaration()));
    meshVertexDec = resources.add(new Gx::VertexDeclaration(device, MeshVertex::declaration()));
    textureVertexDec = resources.add(new Gx::VertexDeclaration(device, TextureVertex::declaration()));
    positionVertexDec = resources.add(new Gx::VertexDeclaration(device, PositionVertex::declaration()));
    rigVertexDec = resources.add(new Gx::VertexDeclaration(device, RigVertex::declaration()));

    colorVertexShader = resources.add(new Gx::VertexShader(device, loadRawData(resourcePath("assets/shaders/colorvertex.dat"))));
    screenVertexShader = resources.add(new Gx::VertexShader(device, loadRawData(resourcePath("assets/shaders/screenvertex.dat"))));
    depthVertexShader = resources.add(new Gx::VertexShader(device, loadRawData(resourcePath("assets/shaders/depthvertex.dat"))));
    meshVertexShader = resources.add(new Gx::VertexShader(device, loadRawData(resourcePath("assets/shaders/meshvertex.dat"))));
    rigVertexShader = resources.add(new Gx::VertexShader(device, loadRawData(resourcePath("assets/shaders/rigvertex.dat"))));
    rigDepthVertexShader = resources.add(new Gx::VertexShader(device, loadRawData(resourcePath("assets/shaders/rigdepthvertex.dat"))));
    skyBoxVertexShader = resources.add(new Gx::VertexShader(device, loadRawData(resourcePath("assets/shaders/skyboxvertex.dat"))));

    depthPixelShader = resources.add(new Gx::PixelShader(device, loadRawData(resourcePath("assets/shaders/depthpixel.dat"))));
    unpackPixelShader = resources.add(new Gx::PixelShader(device, loadRawData(resourcePath("assets/shaders/unpackpixel.dat"))));
    skyBoxPixelShader = resources.add(new Gx::PixelShader(device, loadRawData(resourcePath("assets/shaders/skyboxpixel.dat"))));

    genericBuffer = resources.add(new VertexBuffer(device, 1000 * sizeof(MeshVertex), Gx::Graphics::Usage::Flag::Dynamic, Gx::Graphics::Pool::Default));

    device.setTextureFilter(0, Gx::Graphics::Filter::None);
    device.setTextureFilter(1, Gx::Graphics::Filter::None);
    device.setTextureFilter(2, Gx::Graphics::Filter::Anisotropic);
    device.setTextureFilter(3, Gx::Graphics::Filter::Anisotropic);
    device.setTextureFilter(4, Gx::Graphics::Filter::Linear);
}

Graphics::~Graphics() = default;

void Graphics::reset()
{
    for(auto &r: resources)
    {
        if(r.isDeviceBound()) r.release();
    }

    device.reset();

    for(auto &r: resources)
    {
        if(r.isDeviceBound()) r.reset(device);
    }

    deviceReset();
}

void Graphics::setVertexShader(Gx::VertexShader &shader)
{
    device.setVertexShader(shader);
    cache.get<Cache>().vs = &shader;
}

void Graphics::setVertexShader()
{
    device.setVertexShader();
    cache.get<Cache>().vs = nullptr;
}

void Graphics::setPixelShader(Gx::PixelShader &shader)
{
    device.setPixelShader(shader);
    cache.get<Cache>().ps = &shader;
}

void Graphics::setPixelShader()
{
    device.setPixelShader();
    cache.get<Cache>().ps = nullptr;
}

Gx::VertexShader *Graphics::currentVertexShader()
{
    return cache.get<Cache>().vs;
}

Gx::PixelShader *Graphics::currentPixelShader()
{
    return cache.get<Cache>().ps;
}

