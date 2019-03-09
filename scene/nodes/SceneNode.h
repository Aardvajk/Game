#ifndef SCENENODE_H
#define SCENENODE_H

#include "scene/RenderTypes.h"

#include <pcx/non_copyable.h>

class Graphics;
class SceneParams;

class SceneNode : public pcx::non_copyable
{
public:
    virtual ~SceneNode() = default;

    virtual bool pass(RenderPass type) const = 0;
    virtual RenderType type() const = 0;

    virtual void render(RenderPass pass, Graphics &graphics, SceneParams &params) const = 0;
};

#endif // SCENENODE_H
