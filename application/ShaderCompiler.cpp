#include "ShaderCompiler.h"

#include "application/ResourcePath.h"

#include <GxCore/GxDebug.h>
#include <GxCore/GxWindows.h>

#include <pcx/str.h>
#include <pcx/flags.h>

#include <vector>
#include <fstream>
#include <string>
#include <stdexcept>
#include <d3dx9.h>

namespace
{

enum class Type
{
    Vertex, Pixel, Null
};

bool compileShader(Type type, const D3DXMACRO *macros, const std::string &sourcePath, const std::string &destPath)
{
    Gx::Debug() << "compiling shader: " << sourcePath << " to " << destPath << "\n";

    ID3DXBuffer *buffer = 0, *errors = 0;

    HRESULT r = D3DXCompileShaderFromFile(sourcePath.c_str(), macros, nullptr, "main", type == Type::Vertex ? "vs_3_0" : "ps_3_0", { }, &buffer, &errors, nullptr);
    if(errors)
    {
        std::string err = std::string(reinterpret_cast<const char*>(errors->GetBufferPointer()));

        Gx::Debug() << "Shader Error: " << err << "\n";

        errors->Release();
        return false;
    }

    if(FAILED(r))
    {
        return false;
    }

    std::ofstream os(destPath.c_str(), std::ios::binary);
    if(!os.is_open())
    {
        Gx::Debug() << "Error: unable to create output - " << destPath << "\n";
        return -1;
    }

    DWORD size = buffer->GetBufferSize();

    os.write(reinterpret_cast<const char*>(&size), sizeof(DWORD));
    os.write(static_cast<const char*>(buffer->GetBufferPointer()), buffer->GetBufferSize());

    return true;
}

bool needsCompiling(const std::string &sourcePath, const std::string &destPath)
{
    HANDLE source = CreateFile(sourcePath.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if(source == INVALID_HANDLE_VALUE)
    {
        return true;
    }

    FILETIME sourceTime;
    GetFileTime(source, NULL, NULL, &sourceTime);

    CloseHandle(source);

    HANDLE data = CreateFile(destPath.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if(data != INVALID_HANDLE_VALUE)
    {
        FILETIME dataTime;
        GetFileTime(data, NULL, NULL, &dataTime);

        CloseHandle(data);

        if(CompareFileTime(&sourceTime, &dataTime) <= 0)
        {
            return false;
        }
    }

    return true;
}

}

void compileVertexShader(const std::string &name)
{
    auto input = resourcePath("assets/shaders/") + name + "vertex.txt";
    auto output = resourcePath("assets/shaders/") + name + std::string("vertex.dat");

    if(needsCompiling(input, output))
    {
        if(!compileShader(Type::Vertex, nullptr, input, output))
        {
            throw std::runtime_error("shader compiler failed");
        }
    }
}

void compilePixelShader(const std::string &name)
{
    auto input = resourcePath("assets/shaders/") + name + "pixel.txt";
    auto output = resourcePath("assets/shaders/") + name + std::string("pixel.dat");

    if(needsCompiling(input, output))
    {
        if(!compileShader(Type::Pixel, nullptr, input, output))
        {
            throw std::runtime_error("shader compiler failed");
        }
    }
}

std::string pixelShaderName(const std::string &name, RenderKey::Features features)
{
    return name + pcx::str("pixel", static_cast<int>(features), ".dat");
}

void compilePixelShaderSet(const std::string &name, RenderKey::Features features)
{
    std::vector<D3DXMACRO> m;

    if(features & RenderKey::Feature::Shadows) m.push_back({ "SHADOWS", nullptr });
    if(features & RenderKey::Feature::Diffuse) m.push_back({ "DIFFUSE", nullptr });
    if(features & RenderKey::Feature::Normal) m.push_back({ "NORMAL", nullptr });

    m.push_back({ });

    auto input = resourcePath("assets/shaders/") + name + "pixel.txt";
    auto output = resourcePath("assets/shaders/") + pixelShaderName(name, features);

    if(needsCompiling(input, output))
    {
        if(!compileShader(Type::Pixel, m.data(), input, output))
        {
            throw std::runtime_error("shader compiler failed");
        }
    }
}
