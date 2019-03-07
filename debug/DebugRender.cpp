#include "DebugRender.h"

#include "graphics/Graphics.h"
#include "graphics/VertexStream.h"

#include "graphics/vertices/TextureVertex.h"

#include <GxGraphics/GxTexture.h>

void DebugRender::drawScreenTexture(Graphics &graphics, const Gx::Rect &rect, const Gx::Texture &texture)
{
    graphics.device.setVertexDeclaration(*graphics.textureVertexDec);

    graphics.device.setVertexShader(*graphics.screenShader);
    graphics.screenShader->setFloat(graphics.device, "dx", graphics.size.width);
    graphics.screenShader->setFloat(graphics.device, "dy", graphics.size.height);

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

    graphics.genericBuffer->renderTriangleList(graphics.device, sizeof(TextureVertex));

    graphics.device.setZBufferEnable(true);
    graphics.device.setTexture(0);
}
