#include "Model.h"

#include "scene/Scene.h"
#include "scene/nodes/StaticMeshNode.h"

#include "graphics/VertexBuffer.h"

#include <GxMaths/GxVector.h>
#include <GxMaths/GxMatrix.h>

#include <GxPhysics/GxPhysicsModel.h>
#include <GxPhysics/GxBody.h>
#include <GxPhysics/GxShapes/GxPolyhedronShape.h>

#include <map>

#include <pcx/datastream.h>

namespace
{

pcx::data_istream &operator>>(pcx::data_istream &ds, Gx::Vec3 &v)
{
    return ds >> v.x >> v.y >> v.z;
}

pcx::data_istream &operator>>(pcx::data_istream &ds, Gx::PolyhedronShape::Face &v)
{
    v.clear();

    auto n = ds.get<std::size_t>();
    for(std::size_t i = 0; i < n; ++i) v.push_back(ds.get<std::size_t>());

    return ds;
}

}

Model::Model()
{
}

Model::~Model()
{
}

bool Model::load(Graphics &graphics, Scene &scene, Gx::PhysicsModel &physics, const std::string &path)
{
    pcx::data_ifstream ds(path);
    if(!ds.is_open())
    {
        return false;
    }

    std::map<std::string, VertexBuffer*> bufferMap;

    ds.get<int>();

    auto tag = ds.get<std::string>();
    while(tag.length())
    {
        if(tag == "staticpolyhedron")
        {
            auto pos = ds.get<Gx::Vec3>();

            auto vs = ds.get<std::vector<Gx::Vec3> >();
            auto fs = ds.get<std::vector<Gx::PolyhedronShape::Face> >();

            bodies.push_back(physics.createBody(new Gx::PolyhedronShape(vs, fs), Gx::Matrix::translation(pos), 0));
        }
        else if(tag == "internalmesh")
        {
            auto id = ds.get<std::string>();

            buffers.push_back(graphics.resources.add(new VertexBuffer(graphics.device, ds, { }, Gx::Graphics::Pool::Managed)));
            bufferMap[id] = buffers.back().get();
        }
        else if(tag == "staticmeshinstance")
        {
            auto id = ds.get<std::string>();
            auto pos = ds.get<Gx::Vec3>();

            nodes.push_back(scene.addNode(new StaticMeshNode(bufferMap[id], Gx::Matrix::translation(pos))));
        }

        tag = ds.get<std::string>();
    }

    return true;
}

