#include "DebugLines.h"

#include "graphics/Graphics.h"
#include "graphics/VertexBuffer.h"
#include "graphics/VertexStream.h"

#include "graphics/vertices/ColorVertex.h"

#include "scene/SceneParams.h"

#include "debug/DebugMesh.h"

#include <GxMaths/GxVector.h>
#include <GxMaths/GxColor.h>

#include <GxGraphics/GxShader.h>

#include <GxPhysics/GxPhysicsModel.h>
#include <GxPhysics/GxBody.h>
#include <GxPhysics/GxShapes/GxPolyhedronShape.h>
#include <GxPhysics/GxShapes/GxCapsuleShape.h>
#include <GxPhysics/GxShapes/GxSphereShape.h>

#include <vector>
#include <unordered_set>

#include <GxCore/GxDebug.h>

namespace
{

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

}

void DebugLines::clear()
{
    lines.clear();
}

void DebugLines::addLine(const Gx::Vec3 &start, const Gx::Vec3 &end, const Gx::Color &color)
{
    lines.push_back({ start, end, color });
}

void DebugLines::addPhysics(const Gx::PhysicsModel &physics)
{
    for(int index = 0; index < physics.count(); ++index)
    {
        auto &body = physics.body(index);
        auto tr = body.matrix();

        DebugMesh m;

        if(auto c = dynamic_cast<const Gx::PolyhedronShape*>(&body.shape()))
        {
            m = DebugMesh(c->vertices(), c->faces());
        }
        else if(auto c = dynamic_cast<const Gx::CapsuleShape*>(&body.shape()))
        {
            m = DebugMesh::capsule(8, 8, c->radius(), c->height());
        }
        else if(auto c = dynamic_cast<const Gx::SphereShape*>(&body.shape()))
        {
            m = DebugMesh::capsule(8, 8, c->radius(), c->radius() * 2);
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
            addLine(m.vs[edge.first].transformedCoord(tr), m.vs[edge.second].transformedCoord(tr), { 1, 1, 1 });
        }
    }
}

void DebugLines::render(Graphics &graphics, const SceneParams &params)
{
    graphics.device.setVertexDeclaration(*graphics.colorVertexDec);
    graphics.setVertexShader(*graphics.colorVertexShader);

    graphics.currentVertexShader()->setMatrix(graphics.device, "world", Gx::Matrix::identity());
    graphics.currentVertexShader()->setMatrix(graphics.device, "viewproj", params.viewMatrix * params.projMatrix);

    graphics.device.setZBufferEnable(false);
    graphics.device.setZWriteEnable(false);

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

    graphics.device.setZBufferEnable(true);
    graphics.device.setZWriteEnable(true);
}
