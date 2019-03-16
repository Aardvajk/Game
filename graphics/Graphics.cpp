#include "Graphics.h"

#include "application/ResourcePath.h"

#include "graphics/VertexBuffer.h"
#include "graphics/vertices/ColorVertex.h"
#include "graphics/vertices/MeshVertex.h"
#include "graphics/vertices/TextureVertex.h"

#include <GxGraphics/GxGraphicsTypes.h>
#include <GxGraphics/GxVertexDeclaration.h>
#include <GxGraphics/GxShader.h>
#include <GxGraphics/GxTexture.h>

#include <fstream>

namespace
{

std::vector<char> load(const std::string &path)
{
    std::ifstream is(path.c_str(), std::ios::binary);
    if(!is.is_open())
    {
        throw std::runtime_error("unable to open - " + path);
    }

    DWORD n;
    is.read(reinterpret_cast<char*>(&n), sizeof(DWORD));

    std::vector<char> bs(n);
    is.read(bs.data(), n);

    return bs;
}

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

    colorVertexShader = resources.add(new Gx::VertexShader(device, load(resourcePath("assets/shaders/colorvertex.dat"))));
    meshVertexShader = resources.add(new Gx::VertexShader(device, load(resourcePath("assets/shaders/meshvertex.dat"))));
    screenVertexShader = resources.add(new Gx::VertexShader(device, load(resourcePath("assets/shaders/screenvertex.dat"))));
    depthVertexShader = resources.add(new Gx::VertexShader(device, load(resourcePath("assets/shaders/depthvertex.dat"))));
    surfaceVertexShader = resources.add(new Gx::VertexShader(device, load(resourcePath("assets/shaders/surfacevertex.dat"))));

    depthPixelShader = resources.add(new Gx::PixelShader(device, load(resourcePath("assets/shaders/depthpixel.dat"))));
    unpackPixelShader = resources.add(new Gx::PixelShader(device, load(resourcePath("assets/shaders/unpackpixel.dat"))));
    shadowPixelShader = resources.add(new Gx::PixelShader(device, load(resourcePath("assets/shaders/shadowpixel.dat"))));

    genericBuffer = resources.add(new VertexBuffer(device, 1000 * sizeof(MeshVertex), Gx::Graphics::Usage::Flag::Dynamic, Gx::Graphics::Pool::Default));
    genericTexture = resources.add(new Gx::Texture(device, resourcePath("assets/textures/generic.png"), { { }, 0, { }, Gx::Graphics::Format::A8R8G8B8, Gx::Graphics::Pool::Managed }));
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

