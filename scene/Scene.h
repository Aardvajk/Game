#ifndef SCENE_H
#define SCENE_H

#include "scene/RenderTypes.h"

#include "graphics/Graphics.h"

#include <vector>

#include <pcx/non_copyable.h>

class SceneParams;
class SceneNode;

namespace Gx
{

class Texture;
class DepthStencilSurface;

}

class Scene : public pcx::non_copyable
{
public:
    explicit Scene(Graphics &graphics);

    template<typename T> T *addNode(T *node){ ns.push_back(node); return node; }

    void render(Graphics &graphics, SceneParams &params);

private:
    void render(RenderPass pass, Graphics &graphics, SceneParams &params);

    void beginType(RenderPass pass, RenderType type, Graphics &graphics, SceneParams &params);
    void endType(Graphics &graphics);

    Graphics::Handle<Gx::Texture> playerDepthTex;
    Graphics::Handle<Gx::DepthStencilSurface> playerDepthStencil;

    Graphics::Handle<Gx::Texture> mainDepthTex;
    Graphics::Handle<Gx::DepthStencilSurface> mainDepthStencil;

    std::vector<SceneNode*> ns;
};

#endif // SCENE_H
