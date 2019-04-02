#include "DebugRender.h"

#include "graphics/Graphics.h"
#include "graphics/VertexBuffer.h"
#include "graphics/VertexStream.h"

#include "graphics/vertices/ColorVertex.h"
#include "graphics/vertices/TextureVertex.h"

#include "scene/SceneParams.h"

#include "debug/DebugMesh.h"

#include <GxMaths/GxVector.h>
#include <GxMaths/GxColor.h>
#include <GxMaths/GxRect.h>

#include <GxGraphics/GxShader.h>
#include <GxGraphics/GxTexture.h>

#include <GxPhysics/GxPhysicsModel.h>
#include <GxPhysics/GxBody.h>
#include <GxPhysics/GxShapes/GxPolyhedronShape.h>
#include <GxPhysics/GxShapes/GxCapsuleShape.h>
#include <GxPhysics/GxShapes/GxSphereShape.h>
#include <GxPhysics/GxShapes/GxConeShape.h>

#include <vector>
#include <unordered_set>

namespace
{

struct Point
{
    Gx::Vec3 position;
    Gx::Color color;
};

std::vector<Point> points;

struct Line
{
    Gx::Vec3 start;
    Gx::Vec3 end;
    Gx::Color color;
};

std::vector<Line> lines;

struct EdgeHash
{
    std::size_t operator()(const std::pair<std::size_t, std::size_t> &v) const { return std::hash<std::size_t>()(v.first) ^ std::hash<std::size_t>()(v.second); }
    static std::pair<std::size_t, std::size_t> edge(std::size_t a, std::size_t b){ return std::make_pair(a < b ? a : b, a < b ? b : a); }
};

void beginRender(Graphics &graphics, const SceneParams &params)
{
    graphics.device.setVertexDeclaration(*graphics.colorVertexDec);
    graphics.setVertexShader(*graphics.colorVertexShader);

    graphics.currentVertexShader()->setMatrix(graphics.device, "world", Gx::Matrix::identity());
    graphics.currentVertexShader()->setMatrix(graphics.device, "viewproj", params.viewMatrix * params.projMatrix);

    graphics.device.setZBufferEnable(false);
    graphics.device.setZWriteEnable(false);
}

void endRender(Graphics &graphics)
{
    graphics.device.setZBufferEnable(true);
    graphics.device.setZWriteEnable(true);
}

}

void DebugPoints::clear()
{
    points.clear();
}

void DebugPoints::addPoint(const Gx::Vec3 &position, const Gx::Color &color)
{
    points.push_back({ position, color });
}

void DebugPoints::render(Graphics &graphics, const SceneParams &params)
{
    beginRender(graphics, params);
    graphics.device.setPointSize(8);

    std::size_t chunk = 200;
    std::size_t start = 0;

    while(start < points.size())
    {
        if(auto vs = VertexStream(*graphics.genericBuffer))
        {
            for(std::size_t index = start; index < points.size() && index < start + chunk; ++index)
            {
                vs << points[index].position << Gx::Rgba(points[index].color);
            }
        }

        graphics.genericBuffer->renderPointList(graphics.device, sizeof(ColorVertex));

        start += chunk;
    }

    graphics.device.setPointSize(1);
    endRender(graphics);
}

void DebugLines::clear()
{
    lines.clear();
}

void DebugLines::addLine(const Gx::Vec3 &start, const Gx::Vec3 &end, const Gx::Color &color)
{
    lines.push_back({ start, end, color });
}

void DebugLines::addShape(const Gx::Shape &shape, const Gx::Matrix &tr, const Gx::Color &color)
{
    DebugMesh m;

    if(auto c = dynamic_cast<const Gx::PolyhedronShape*>(&shape))
    {
        m = DebugMesh(c->vertices(), c->faces());
    }
    else if(auto c = dynamic_cast<const Gx::CapsuleShape*>(&shape))
    {
        m = DebugMesh::capsule(8, 8, c->radius(), c->height());
    }
    else if(auto c = dynamic_cast<const Gx::SphereShape*>(&shape))
    {
        m = DebugMesh::sphere(8, 8, c->radius());
    }
    else if(auto c = dynamic_cast<const Gx::ConeShape*>(&shape))
    {
        m = DebugMesh::cone(8, c->radius(), c->height());
    }

    std::unordered_set<std::pair<std::size_t, std::size_t>, EdgeHash> edges;

    for(auto &face: m.fs)
    {
        for(std::size_t i = 0; i < face.size(); ++i)
        {
            edges.insert(EdgeHash::edge(face[i], face[i < face.size() - 1 ? i + 1 : 0]));
        }
    }

    for(auto edge: edges)
    {
        addLine(m.vs[edge.first].transformedCoord(tr), m.vs[edge.second].transformedCoord(tr), color);
    }
}

void DebugLines::addPhysics(const Gx::PhysicsModel &physics, const Gx::Color &color)
{
    for(int index = 0; index < physics.bodyCount(); ++index)
    {
        auto &body = physics.body(index);
        auto tr = body.matrix();

        addShape(body.shape(), tr, color);
    }
}

void DebugLines::render(Graphics &graphics, const SceneParams &params)
{
    beginRender(graphics, params);

    std::size_t chunk = 200;
    std::size_t start = 0;

    while(start < lines.size())
    {
        if(auto vs = VertexStream(*graphics.genericBuffer))
        {
            for(std::size_t index = start; index < lines.size() && index < start + chunk; ++index)
            {
                vs << lines[index].start << Gx::Rgba(lines[index].color);
                vs << lines[index].end << Gx::Rgba(lines[index].color);
            }
        }

        graphics.genericBuffer->renderLineList(graphics.device, sizeof(ColorVertex));

        start += chunk;
    }

    endRender(graphics);
}

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
