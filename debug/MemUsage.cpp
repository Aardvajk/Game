#include "MemUsage.h"

#include <GxCore/GxWindows.h>

#include <psapi.h>

float MemUsage::current()
{
    PROCESS_MEMORY_COUNTERS info;
    GetProcessMemoryInfo(GetCurrentProcess(), &info, sizeof(info));

    return (static_cast<float>(info.WorkingSetSize) / 1024.0f) / 1024.0f;
}

float MemUsage::peak()
{
    PROCESS_MEMORY_COUNTERS info;
    GetProcessMemoryInfo(GetCurrentProcess(), &info, sizeof(info));

    return (static_cast<float>(info.PeakWorkingSetSize) / 1024.0f) / 1024.0f;
}
