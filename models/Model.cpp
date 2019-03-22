#include "Model.h"

#include "application/ResourcePath.h"
#include "application/ShaderCompiler.h"

#include "scene/Scene.h"

#include "scene/nodes/StaticMeshNode.h"
#include "scene/nodes/SkyBoxNode.h"

#include "graphics/VertexBuffer.h"
#include "graphics/SkyBox.h"

#include "entities/Entity.h"
#include "entities/pc/Pc.h"

#include <GxMaths/GxVector.h>
#include <GxMaths/GxMatrix.h>

#include <GxGraphics/GxTexture.h>
#include <GxGraphics/GxCubeMap.h>
#include <GxGraphics/GxShader.h>

#include <GxPhysics/GxPhysicsModel.h>
#include <GxPhysics/GxBody.h>
#include <GxPhysics/GxShapes/GxPolyhedronShape.h>

#include <unordered_set>

#include <pcx/datastream.h>
#include <pcx/str.h>

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

Model::Model() = default;
Model::~Model() = default;

bool Model::load(Graphics &graphics, Scene &scene, Gx::PhysicsModel &physics, const std::string &path)
{
    pcx::data_ifstream ds(path);
    if(!ds.is_open())
    {
        return false;
    }

    std::unordered_map<std::string, VertexBuffer*> bufferMap;
    std::unordered_map<std::string, Gx::Texture*> textureMap;

    std::unordered_set<RenderKey::Features> featureSet;
    featureSet.insert(static_cast<RenderKey::Feature>(0));

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
        else if(tag == "texture")
        {
            auto id = ds.get<std::string>();
            auto path = ds.get<std::string>();

            textures.push_back(graphics.resources.add(new Gx::Texture(graphics.device, resourcePath(pcx::str("assets/textures/", path)), { { }, 0, { }, Gx::Graphics::Format::A8R8G8B8, Gx::Graphics::Pool::Managed})));
            textureMap[id] = textures.back().get();
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
            auto diffuse = ds.get<std::string>();
            auto pos = ds.get<Gx::Vec3>();

            RenderKey key(true, textureMap[diffuse]);
            featureSet.insert(key.features());

            nodes.push_back(scene.addNode(new StaticMeshNode(bufferMap[id], key, Gx::Matrix::translation(pos))));
        }

        tag = ds.get<std::string>();
    }

    if(true)
    {
        buffers.push_back(graphics.resources.add(skyBoxBuffer(graphics, 10)));

        std::vector<std::string> files = { "right", "left", "top", "bottom", "front", "back" };

        std::vector<std::string> paths;
        for(auto s: files)
        {
            paths.push_back(resourcePath(pcx::str("assets/skyboxes/skybox/", s, ".png")));
        }

        cubeMaps.push_back(graphics.resources.add(new Gx::CubeMap(graphics.device, paths, { 256, 0, { }, Gx::Graphics::Format::X8R8G8B8, Gx::Graphics::Pool::Managed })));

        nodes.push_back(scene.addNode(new SkyBoxNode(buffers.back().get(), cubeMaps.back().get())));
    }

    for(auto perm: featureSet)
    {
        auto data = loadRawData(resourcePath(pcx::str("assets/shaders/", pixelShaderName("mesh", perm))));

        pixelShaders.push_back(graphics.resources.add(new Gx::PixelShader(graphics.device, data)));
        pixelShaderMapping[perm] = pixelShaders.back().get();
    }

    entities.push_back(new Pc(graphics, scene));

    return true;
}

void Model::addEntity(Entity *e)
{
    entities.push_back(e);
}

void Model::update(const FrameParams &params, Events &events, Gx::PhysicsModel &physics, float delta)
{
    for(auto &e: entities)
    {
        e.update(params, events, physics, delta);
    }
}

void Model::prepareScene(SceneParams &params, float blend)
{
    for(auto &e: entities)
    {
        e.prepareScene(params, blend);
    }
}

Gx::PixelShader &Model::pixelShader(RenderKey::Features features)
{
    return *pixelShaderMapping[features];
}

