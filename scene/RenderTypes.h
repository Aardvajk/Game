#ifndef RENDERTYPES_H
#define RENDERTYPES_H

enum class RenderPass
{
    Normal,
    PlayerDepth,
    MainDepth
};

enum class RenderType
{
    Color,
    Player,
    Surface,
    Null
};

#endif // RENDERTYPES_H
