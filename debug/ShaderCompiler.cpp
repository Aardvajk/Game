#include "ShaderCompiler.h"

#include "application/ResourcePath.h"

#include <GxCore/GxDebug.h>
#include <GxCore/GxWindows.h>

#include <fstream>
#include <vector>
#include <string>
#include <stdexcept>
#include <d3dx9.h>

namespace
{

enum class Type
{
    Vertex, Pixel, Null
};

bool compileShader(Type type, const std::string &sourcePath, const std::string &destPath)
{
    ID3DXBuffer *buffer = 0, *errors = 0;

    HRESULT r = D3DXCompileShaderFromFile(sourcePath.c_str(), nullptr, 0, "main", type == Type::Vertex ? "vs_3_0" : "ps_3_0", 0, &buffer, &errors, NULL);
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

void compileShaders()
{
    WIN32_FIND_DATA fd;
    HANDLE h = FindFirstFile(resourcePath("assets/shaders/*.txt").c_str(), &fd);

    std::vector<std::string> files;
    do
    {
        files.push_back(fd.cFileName);
    }
    while(FindNextFile(h, &fd) != 0);

    CloseHandle(h);

    for(auto input: files)
    {
        if(input.length() > 10)
        {
            Type type = Type::Null;
            if(input.substr(input.length() - 10, 6) == "vertex")
            {
                type = Type::Vertex;
            }
            else if(input.substr(input.length() - 9, 5) == "pixel")
            {
                type = Type::Pixel;
            }

            input = resourcePath("assets/shaders/") + input;
            auto output = input.substr(0, input.length() - 3) + std::string("dat");

            if(type != Type::Null)
            {
                if(needsCompiling(input, output))
                {
                    if(!compileShader(type, input, output))
                    {
                        throw std::runtime_error("shader compiler failed");
                    }
                }
            }
        }
    }
}
