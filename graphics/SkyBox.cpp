#include "SkyBox.h"

#include "graphics/Graphics.h"
#include "graphics/VertexBuffer.h"

#include "graphics/vertices/PositionVertex.h"

#include <GxMaths/GxVector.h>

#include <GxPhysics/GxShapes/GxPolyhedronShape.h>

#include <pcx/scoped_ptr.h>
#include <pcx/datastream.h>

VertexBuffer *skyBoxBuffer(Graphics &graphics, float size)
{
    std::vector<Gx::Vec3> vertices;
    std::vector<Gx::PolyhedronShape::Face> faces;

    auto d = size / 2;

    vertices.push_back({ -d, d, -d });
    vertices.push_back({ d, d, -d });
    vertices.push_back({ d, -d, -d });
    vertices.push_back({ -d, -d, -d });

    vertices.push_back({ -d, d, d });
    vertices.push_back({ d, d, d });
    vertices.push_back({ d, -d, d });
    vertices.push_back({ -d, -d, d });

    faces.push_back({ 0, 1, 2, 3 });
    faces.push_back({ 1, 5, 6, 2 });
    faces.push_back({ 4, 0, 3, 7 });
    faces.push_back({ 5, 4, 7, 6 });
    faces.push_back({ 4, 5, 1, 0 });
    faces.push_back({ 3, 2, 6, 7 });

    pcx::data_osstream ds;

    for(std::size_t i = 0; i < faces.size(); ++i)
    {
        for(std::size_t j = 1; j < faces[i].size() - 1; ++j)
        {
            ds << vertices[faces[i][0]];
            ds << vertices[faces[i][j + 1]];
            ds << vertices[faces[i][j]];
        }
    }

    return new VertexBuffer(graphics.device, ds.data(), { }, Gx::Graphics::Pool::Managed);
}
