#include "RenderKey.h"

RenderKey::RenderKey() : s(false), d(nullptr), n(nullptr), f({ })
{
}

RenderKey::RenderKey(bool shadows, const Gx::Texture *diffuse, const Gx::Texture *normal) : s(shadows), d(diffuse), n(normal), f({ })
{
    if(shadows) f |= RenderKey::Feature::Shadows;
    if(diffuse) f |= RenderKey::Feature::Diffuse;
    if(normal) f |= RenderKey::Feature::Normal;
}

bool operator==(const RenderKey &a, const RenderKey &b)
{
    return a.shadows() == b.shadows() && a.diffuse() == b.diffuse() && a.normal() == b.normal();
}

bool operator!=(const RenderKey &a, const RenderKey &b)
{
    return a.shadows() != b.shadows() || a.diffuse() != b.diffuse() || a.normal() != b.normal();
}

