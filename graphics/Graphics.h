#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <GxGraphics/GxExclusiveGraphicsDevice.h>
#include <GxGraphics/GxGraphicsResource.h>
#include <GxGraphics/GxVertexDeclaration.h>
#include <GxGraphics/GxShader.h>

#include <pcx/resource_map.h>
#include <pcx/signals.h>

class Graphics
{
public:
    Graphics(HWND hw, const Gx::DisplaySettings &settings);

    template<typename T> using Handle = pcx::resource_handle<Gx::GraphicsResource, T>;

    void reset();

    pcx::signal<> deviceReset;

    Gx::ExclusiveGraphicsDevice device;
    pcx::resource_map<Gx::GraphicsResource> resources;

    Handle<Gx::VertexDeclaration> meshVertexDec;
    Handle<Gx::VertexShader> meshShader;
};

#endif // GRAPHICS_H
