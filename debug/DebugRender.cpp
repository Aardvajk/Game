#include "DebugRender.h"

#include "graphics/Graphics.h"
#include "graphics/VertexStream.h"

#include "graphics/vertices/MeshVertex.h"

void DebugRender::drawScreenTexture(Graphics &graphics, const Gx::Texture &texture)
{
    graphics.device.setVertexDeclaration(*graphics.meshVertexDec);

    graphics.device.setVertexShader(*graphics.screenShader);
    graphics.screenShader->setFloat(graphics.device, "dx", 1024.0f);
    graphics.screenShader->setFloat(graphics.device, "dy", 768.0f);

    if(auto os = VertexStream(*graphics.genericBuffer))
    {
        Gx::Vec2 pos(5, 5);
        Gx::Vec2 size(256, 265);
        Gx::Color color(1, 1, 1);

        os << Gx::Vec3(pos.x, pos.y, 0) << Gx::Vec3() << Gx::Rgba(color) << Gx::Vec2(0, 0);
        os << Gx::Vec3(pos.x + size.x, pos.y, 0) << Gx::Vec3() << Gx::Rgba(color) << Gx::Vec2(1, 0);
        os << Gx::Vec3(pos.x, pos.y + size.y, 0) << Gx::Vec3() << Gx::Rgba(color) << Gx::Vec2(0, 1);

        os << Gx::Vec3(pos.x + size.x, pos.y, 0) << Gx::Vec3() << Gx::Rgba(color) << Gx::Vec2(1, 0);
        os << Gx::Vec3(pos.x + size.x, pos.y + size.y, 0) << Gx::Vec3() << Gx::Rgba(color) << Gx::Vec2(1, 1);
        os << Gx::Vec3(pos.x, pos.y + size.y, 0) << Gx::Vec3() << Gx::Rgba(color) << Gx::Vec2(0, 1);
    }

    graphics.device.setZBufferEnable(false);
    graphics.device.setTexture(0, texture);

    graphics.genericBuffer->renderTriangleList(graphics.device, sizeof(MeshVertex));

    graphics.device.setZBufferEnable(true);
    graphics.device.setTexture(0);
}
