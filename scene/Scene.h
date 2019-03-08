#ifndef SCENE_H
#define SCENE_H

#include "scene/RenderTypes.h"

#include <vector>

#include <pcx/non_copyable.h>

class Graphics;
class SceneParams;
class SceneNode;

class Scene : public pcx::non_copyable
{
public:
    Scene();

    template<typename T> T *addNode(T *node){ ns.push_back(node); return node; }

    void render(RenderPass pass, Graphics &graphics, const SceneParams &params);

    void beginType(RenderPass pass, RenderType type, Graphics &graphics, const SceneParams &params);
    void endType(Graphics &graphics);

private:
    std::vector<SceneNode*> ns;
};

#endif // SCENE_H
