#ifndef MODEL_H
#define MODEL_H

#include "graphics/Graphics.h"
#include "graphics/VertexBuffer.h"

#include "scene/nodes/SceneNode.h"

#include <string>
#include <vector>

#include <GxPhysics/GxBody.h>

#include <pcx/non_copyable.h>
#include <pcx/ptr_vector.h>

class Scene;
class SceneNode;

namespace Gx
{

class PhysicsModel;

}

class Model : public pcx::non_copyable
{
public:
    Model() = default;

    bool load(Graphics &graphics, Scene &scene, Gx::PhysicsModel &physics, const std::string &path);

private:
    std::vector<Graphics::Handle<VertexBuffer> > buffers;
    pcx::ptr_vector<SceneNode> nodes;
    pcx::ptr_vector<Gx::Body> bodies;
};

#endif // MODEL_H
