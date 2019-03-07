#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <GxGraphics/GxExclusiveGraphicsDevice.h>

#include <pcx/resource_map.h>
#include <pcx/signals.h>

namespace Gx
{

class GraphicsResource;
class VertexDeclaration;
class VertexShader;

}

class VertexBuffer;

class Graphics
{
public:
    Graphics(HWND hw, const Gx::DisplaySettings &settings);
    ~Graphics();

    template<typename T> using Handle = pcx::resource_handle<Gx::GraphicsResource, T>;

    void reset();

    pcx::signal<> deviceReset;

    Gx::ExclusiveGraphicsDevice device;
    pcx::resource_map<Gx::GraphicsResource> resources;

    Handle<Gx::VertexDeclaration> colorVertexDec;
    Handle<Gx::VertexDeclaration> meshVertexDec;
    Handle<Gx::VertexDeclaration> textureVertexDec;

    Handle<Gx::VertexShader> colorShader;
    Handle<Gx::VertexShader> meshShader;
    Handle<Gx::VertexShader> screenShader;

    Handle<VertexBuffer> genericBuffer;

    Gx::SizeF size;
};

#endif // GRAPHICS_H
