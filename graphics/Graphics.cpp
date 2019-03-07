#include "Graphics.h"

#include "application/ResourcePath.h"

#include "graphics/vertices/ColorVertex.h"
#include "graphics/vertices/MeshVertex.h"
#include "graphics/vertices/TextureVertex.h"

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

}

Graphics::Graphics(HWND hw, const Gx::DisplaySettings &settings) : device(hw, settings), size(settings.size)
{
    colorVertexDec = resources.add(new Gx::VertexDeclaration(device, ColorVertex::declaration()));
    meshVertexDec = resources.add(new Gx::VertexDeclaration(device, MeshVertex::declaration()));
    textureVertexDec = resources.add(new Gx::VertexDeclaration(device, TextureVertex::declaration()));

    colorShader = resources.add(new Gx::VertexShader(device, load(resourcePath("assets/colorvertex.dat"))));
    meshShader = resources.add(new Gx::VertexShader(device, load(resourcePath("assets/meshvertex.dat"))));
    screenShader = resources.add(new Gx::VertexShader(device, load(resourcePath("assets/screenvertex.dat"))));

    genericBuffer = resources.add(new VertexBuffer(device, 1000 * sizeof(MeshVertex), Gx::Graphics::Usage::Flag::Dynamic, Gx::Graphics::Pool::Default));
}

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

