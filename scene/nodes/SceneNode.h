#ifndef SCENENODE_H
#define SCENENODE_H

#include <pcx/non_copyable.h>

class SceneNode : public pcx::non_copyable
{
public:
    virtual ~SceneNode() = default;
};

#endif // SCENENODE_H
