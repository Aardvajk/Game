#include "ResourcePath.h"

#include <GxCore/GxWindows.h>
#include <GxCore/GxDebug.h>

#include <vector>
#include <cctype>
#include <algorithm>
#include <psapi.h>

namespace
{

std::vector<std::string> split(const std::string &s, const std::string &c)
{
    std::vector<std::string> r;

    std::string b;
    std::string::size_type i = 0;

    while(i < s.length())
    {
        if(c.find(s[i]) != std::string::npos)
        {
            if(i)
            {
                r.push_back(b);
                b.clear();
            }
        }
        else
        {
            b.push_back(s[i]);
        }

        ++i;
    }

    if(!b.empty())
    {
        r.push_back(b);
    }

    return r;
}

std::string join(const std::vector<std::string> &v, char c)
{
    std::string r;
    for(auto s: v)
    {
        r += s;
        r += c;
    }

    return r;
}

}

std::string resourcePath(const std::string &path)
{
    auto params = split(GetCommandLine(), " ");
    if(std::find(params.begin(), params.end(), "-ide") != params.end())
    {
        return std::string("../Game/") + path;
    }

    char buffer[MAX_PATH + 1];
    GetModuleFileName(GetModuleHandle(NULL), buffer, MAX_PATH);

    auto sections = split(buffer, "\\/");
    sections.pop_back();

    if(sections.empty())
    {
        return path;
    }

    if(sections.back() == "release")
    {
        return std::string("../../Game/") + path;
    }

    return join(sections, '/') + path;
}
