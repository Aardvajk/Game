#ifndef SCENE_H
#define SCENE_H

#include "scene/RenderTypes.h"

#include "graphics/Graphics.h"

#include <vector>

#include <pcx/non_copyable.h>

class Model;
class SceneParams;
class SceneNode;
class RenderKey;

namespace Gx
{

class Texture;
class DepthStencilSurface;

}

class Scene : public pcx::non_copyable
{
public:
    Scene(Model &model, Graphics &graphics);
    ~Scene();

    template<typename T> T *addNode(T *node){ ns.push_back(node); return node; }

    void render(Graphics &graphics, SceneParams &params);

private:
    void render(RenderPass pass, Graphics &graphics, SceneParams &params);

    void beginType(RenderPass pass, RenderType type, const RenderKey &key, Graphics &graphics, SceneParams &params);
    void endType(Graphics &graphics);

    Model &model;

    Graphics::Handle<Gx::Texture> objectDepthTex;
    Graphics::Handle<Gx::DepthStencilSurface> objectDepthStencil;

    Graphics::Handle<Gx::Texture> mainDepthTex;
    Graphics::Handle<Gx::DepthStencilSurface> mainDepthStencil;

    std::vector<SceneNode*> ns;
};

#endif // SCENE_H
