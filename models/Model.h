#ifndef MODEL_H
#define MODEL_H

#include "graphics/Graphics.h"

#include "scene/RenderKey.h"

#include <string>
#include <vector>
#include <unordered_map>

#include <pcx/non_copyable.h>
#include <pcx/ptr_vector.h>

class Scene;
class SceneNode;
class SceneParams;
class Entity;
class FrameParams;
class Events;

namespace Gx
{

class PhysicsModel;
class Body;

}

class Model : public pcx::non_copyable
{
public:
    Model();
    ~Model();

    bool load(Graphics &graphics, Scene &scene, Gx::PhysicsModel &physics, const std::string &path);

    void addEntity(Entity *e);

    void update(const FrameParams &params, Events &events, Gx::PhysicsModel &physics, float delta);
    void prepareScene(SceneParams &params, float blend);

    Gx::PixelShader &pixelShader(RenderKey::Features features);

private:
    std::vector<Graphics::Handle<VertexBuffer> > buffers;
    std::vector<Graphics::Handle<Gx::Texture> > textures;
    std::vector<Graphics::Handle<Gx::PixelShader> > pixelShaders;

    std::unordered_map<RenderKey::Features, Gx::PixelShader*> pixelShaderMapping;

    pcx::ptr_vector<SceneNode> nodes;
    pcx::ptr_vector<Gx::Body> bodies;
    pcx::ptr_vector<Entity> entities;
};

#endif // MODEL_H
