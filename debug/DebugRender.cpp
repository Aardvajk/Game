#include "DebugRender.h"

#include "graphics/Graphics.h"
#include "graphics/VertexBuffer.h"
#include "graphics/VertexStream.h"

#include "graphics/vertices/TextureVertex.h"

#include <GxMaths/GxRect.h>

#include <GxGraphics/GxShader.h>
#include <GxGraphics/GxTexture.h>

void DebugRender::drawScreenTexture(Graphics &graphics, const Gx::Rect &rect, const Gx::Texture &texture, const Gx::PixelShader *pixelShader)
{
    graphics.device.setVertexDeclaration(*graphics.textureVertexDec);

    graphics.setVertexShader(*graphics.screenVertexShader);
    graphics.currentVertexShader()->setFloat(graphics.device, "dx", graphics.size.width);
    graphics.currentVertexShader()->setFloat(graphics.device, "dy", graphics.size.height);

    if(auto os = VertexStream(*graphics.genericBuffer))
    {
        auto pos = rect.position;
        auto size = (rect.size.width && rect.size.height) ? rect.size : texture.size();
        auto color = Gx::Color(1, 1, 1);

        os << Gx::Vec3(pos.x, pos.y, 0) << Gx::Rgba(color) << Gx::Vec2(0, 0);
        os << Gx::Vec3(pos.x + size.width, pos.y, 0) << Gx::Rgba(color) << Gx::Vec2(1, 0);
        os << Gx::Vec3(pos.x, pos.y + size.height, 0) << Gx::Rgba(color) << Gx::Vec2(0, 1);

        os << Gx::Vec3(pos.x + size.width, pos.y, 0) << Gx::Rgba(color) << Gx::Vec2(1, 0);
        os << Gx::Vec3(pos.x + size.width, pos.y + size.height, 0) << Gx::Rgba(color) << Gx::Vec2(1, 1);
        os << Gx::Vec3(pos.x, pos.y + size.height, 0) << Gx::Rgba(color) << Gx::Vec2(0, 1);
    }

    graphics.device.setZBufferEnable(false);
    graphics.device.setTexture(0, texture);

    if(pixelShader)
    {
        graphics.device.setPixelShader(*pixelShader);
    }

    graphics.genericBuffer->renderTriangleList(graphics.device, sizeof(TextureVertex));

    graphics.device.setZBufferEnable(true);
    graphics.device.setTexture(0);

    if(pixelShader)
    {
        graphics.device.setPixelShader();
    }
}
