#ifndef SCENE_H
#define SCENE_H

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

    void render(Graphics &graphics, const SceneParams &params);

private:
    void beginType(Graphics &graphics, const SceneParams &params);
    void endType(Graphics &graphics);

    std::vector<SceneNode*> ns;
};

#endif // SCENE_H
