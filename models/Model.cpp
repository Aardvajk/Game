#include "Model.h"

#include "scene/Scene.h"

#include "scene/nodes/StaticMeshNode.h"

#include <GxMaths/GxVector.h>
#include <GxMaths/GxMatrix.h>

#include <GxPhysics/GxPhysicsModel.h>
#include <GxPhysics/GxShapes/GxPolyhedronShape.h>

#include <stdexcept>
#include <map>

#include <pcx/datastream.h>

inline pcx::data_istream &operator>>(pcx::data_istream &ds, Gx::Vec3 &v){ return ds >> v.x >> v.y >> v.z; }

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
        if(tag == "mesh")
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
        else
        {
            throw std::runtime_error("invalid level file format");
        }

        tag = ds.get<std::string>();
    }

    bodies.push_back(physics.createBody(Gx::PolyhedronShape::cuboid({ 1, 1, 1 }), Gx::Matrix::translation({ 0, 3, 0 }), 1.0f));
    bodies.push_back(physics.createBody(Gx::PolyhedronShape::cuboid({ 3, 1, 2 }), Gx::Matrix::translation({ -2, 1, 0 }), 1.0f));

    return true;
}

