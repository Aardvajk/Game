#include "ResourcePath.h"

#include <GxCore/GxWindows.h>
#include <GxCore/GxDebug.h>

#include <pcx/split_str.h>
#include <pcx/join_str.h>

#include <vector>
#include <fstream>
#include <algorithm>
#include <psapi.h>

std::string resourcePath(const std::string &path)
{
    auto params = pcx::split_str(GetCommandLine(), " ");
    if(std::find(params.begin(), params.end(), "-ide") != params.end())
    {
        return std::string("../Game/") + path;
    }

    char buffer[MAX_PATH + 1];
    GetModuleFileName(GetModuleHandle(NULL), buffer, MAX_PATH);

    auto sections = pcx::split_str(buffer, "\\/", pcx::split_mode::keep_empty_sections);
    sections.pop_back();

    if(sections.empty())
    {
        return path;
    }

    if(sections.back() == "release")
    {
        return std::string("../../Game/") + path;
    }

    return pcx::join_str(sections, "/") + std::string("/") + path;
}

std::vector<char> loadRawData(const std::string &path)
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
