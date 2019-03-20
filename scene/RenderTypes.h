#ifndef RENDERTYPES_H
#define RENDERTYPES_H

enum class RenderPass
{
    Normal,
    ObjectDepth,
    EnvironmentDepth
};

enum class RenderType
{
    Color,
    Object,
    Environment,
    Null
};

#endif // RENDERTYPES_H
