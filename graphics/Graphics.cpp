#include "Graphics.h"

#include "graphics/vertices/MeshVertex.h"

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

Graphics::Graphics(HWND hw, const Gx::DisplaySettings &settings) : device(hw, settings)
{
    meshVertexDec = resources.add(new Gx::VertexDeclaration(device, MeshVertex::declaration()));
    meshShader = resources.add(new Gx::VertexShader(device, load("C:/Projects/Game/Game/meshvertex.dat")));
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

