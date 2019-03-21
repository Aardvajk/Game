#include "SceneParams.h"

Gx::Matrix SceneParams::depthMatrix(RenderPass pass) const
{
    switch(pass)
    {
        case RenderPass::ObjectDepth: return objectDepthMatrix;
        case RenderPass::EnvironmentDepth: return environmentDepthMatrix;

        default: return { };
    }
}
