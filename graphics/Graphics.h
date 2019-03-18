#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <GxGraphics/GxExclusiveGraphicsDevice.h>

#include <pcx/resource_map.h>
#include <pcx/signals.h>
#include <pcx/aligned_store.h>

namespace Gx
{

class GraphicsResource;
class VertexDeclaration;
class VertexShader;
class PixelShader;
class Texture;

}

class VertexBuffer;

class Graphics
{
public:
    Graphics(HWND hw, const Gx::DisplaySettings &settings);
    ~Graphics();

    template<typename T> using Handle = pcx::resource_handle<Gx::GraphicsResource, T>;

    void reset();

    void setVertexShader(Gx::VertexShader &shader);
    void setVertexShader();

    void setPixelShader(Gx::PixelShader &shader);
    void setPixelShader();

    Gx::VertexShader *currentVertexShader();
    Gx::PixelShader *currentPixelShader();

    pcx::signal<> deviceReset;

    Gx::ExclusiveGraphicsDevice device;
    pcx::resource_map<Gx::GraphicsResource> resources;

    Handle<Gx::VertexDeclaration> colorVertexDec;
    Handle<Gx::VertexDeclaration> meshVertexDec;
    Handle<Gx::VertexDeclaration> textureVertexDec;

    Handle<Gx::VertexShader> colorVertexShader;
    Handle<Gx::VertexShader> meshVertexShader;
    Handle<Gx::VertexShader> screenVertexShader;
    Handle<Gx::VertexShader> depthVertexShader;
    Handle<Gx::VertexShader> surfaceVertexShader;

    Handle<Gx::PixelShader> depthPixelShader;
    Handle<Gx::PixelShader> unpackPixelShader;
    Handle<Gx::PixelShader> shadowPixelShader;

    Handle<VertexBuffer> genericBuffer;

    Gx::SizeF size;

    pcx::aligned_store<16> cache;
};

#endif // GRAPHICS_H
