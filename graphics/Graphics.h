#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <GxGraphics/GxExclusiveGraphicsDevice.h>
#include <GxGraphics/GxGraphicsResource.h>

#include <pcx/resource_map.h>

class Graphics
{
public:
    Graphics(HWND hw, const Gx::DisplaySettings &settings);

    template<typename T> using Ptr = pcx::resource_ptr<Gx::GraphicsResource, T>;
    template<typename T> using Handle = pcx::resource_handle<Gx::GraphicsResource, T>;

    void reset();

    Gx::ExclusiveGraphicsDevice device;
    pcx::resource_map<Gx::GraphicsResource> resources;
};

#endif // GRAPHICS_H
