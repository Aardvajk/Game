#include "application/Application.h"
#include "application/ShaderCompiler.h"

#include <GxCore/GxWindows.h>
#include <GxCore/GxDebug.h>

#include <GxGraphics/GxDisplaySettings.h>

#include <exception>
#include <cstdlib>

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
{
    try
    {
        compileVertexShader("color");
        compileVertexShader("depth");
        compileVertexShader("mesh");
        compileVertexShader("screen");
        compileVertexShader("skybox");
        compileVertexShader("rig");
        compileVertexShader("rigdepth");

        compilePixelShader("depth");
        compilePixelShader("unpack");
        compilePixelShader("skybox");

        for(int i = 0; i < 8; ++i)
        {
            compilePixelShaderSet("lit", static_cast<RenderKey::Feature>(i));
        }

        Application app({ 100, 100 }, { { 1024, 768 }, Gx::DisplaySettings::Flag::Windowed | Gx::DisplaySettings::Flag::VSync, 16 });
        return app.exec();
    }
    
    catch(const std::exception &e)
    {
        MessageBox(NULL, e.what(), "Error", MB_OK);        
        return 1;
    }
}

