#include "application/Application.h"

#include "debug/ShaderCompiler.h"

#include <GxCore/GxWindows.h>
#include <GxCore/GxDebug.h>

#include <GxGraphics/GxDisplaySettings.h>

#include <exception>
#include <cstdlib>

#include <GxMaths/GxVector.h>

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
{
    try
    {
        compileShaders();

        Application app({ { 1024, 768 }, Gx::DisplaySettings::Flag::Windowed | Gx::DisplaySettings::Flag::VSync });
        return app.exec();
    }
    
    catch(const std::exception &e)
    {
        MessageBox(NULL, e.what(), "Error", MB_OK);        
        return 1;
    }
}

