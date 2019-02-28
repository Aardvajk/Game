#ifndef SCENENODE_H
#define SCENENODE_H

#include <pcx/non_copyable.h>

class Graphics;

class SceneNode : public pcx::non_copyable
{
public:
    virtual ~SceneNode() = default;

    virtual void render(Graphics &graphics) const = 0;
};

#endif // SCENENODE_H
