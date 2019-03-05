#include "DebugRender.h"

#include "graphics/Graphics.h"
#include "graphics/VertexBuffer.h"
#include "graphics/VertexStream.h"

#include "graphics/vertices/MeshVertex.h"

#include "scene/SceneParams.h"

#include <GxPhysics/GxPhysicsModel.h>
#include <GxPhysics/GxBody.h>
#include <GxPhysics/GxShapes/GxPolyhedronShape.h>

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
    static std::pair<std::size_t, std::size_t> edge(std::size_t a, std::size_t b){ return a < b ? std::make_pair(a, b) : std::make_pair(b, a); }
};

}

void DebugRender::clear()
{
    lines.clear();
}

void DebugRender::addLine(const Gx::Vec3 &start, const Gx::Vec3 &end, const Gx::Color &color)
{
    lines.push_back({ start, end, color });
}

void DebugRender::addPhysics(const Gx::PhysicsModel &physics)
{
    for(int index = 0; index < physics.count(); ++index)
    {
        auto &body = physics.body(index);
        auto tr = body.transform();

        std::unordered_set<std::pair<std::size_t, std::size_t>, EdgeHash> edges;

        if(auto poly = dynamic_cast<const Gx::PolyhedronShape*>(&body.shape()))
        {
            for(auto &face: poly->faces())
            {
                for(std::size_t i = 0; i < face.size(); ++i)
                {
                    edges.insert(EdgeHash::edge(face[i], face[i < face.size() - 1 ? i + 1 : 0]));
                }
            }

            for(auto edge: edges)
            {
                addLine(poly->vertices()[edge.first].transformedCoord(tr), poly->vertices()[edge.second].transformedCoord(tr), { 1, 1, 1 });
            }
        }
    }
}

void DebugRender::render(Graphics &graphics, const SceneParams &params)
{
    if(auto vs = VertexStream(*graphics.genericBuffer))
    {
        for(auto line: lines)
        {
            vs << line.start << Gx::Vec3(0, 0, 0) << Gx::Rgba(line.color);
            vs << line.end << Gx::Vec3(0, 0, 0) << Gx::Rgba(line.color);
        }
    }

    graphics.device.setVertexDeclaration(*graphics.meshVertexDec);
    graphics.device.setVertexShader(*graphics.colorShader);

    graphics.colorShader->setMatrix(graphics.device, "world", Gx::Matrix::identity());
    graphics.colorShader->setMatrix(graphics.device, "viewproj", params.view * params.proj);

    graphics.device.setZBufferEnable(false);
    graphics.device.setZWriteEnable(false);

    graphics.genericBuffer->renderLineList(graphics.device, sizeof(MeshVertex));

    graphics.device.setZBufferEnable(true);
    graphics.device.setZWriteEnable(true);
}
