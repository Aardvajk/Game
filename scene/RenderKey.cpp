#include "RenderKey.h"

namespace
{

RenderKey::Features makeFeatures(const RenderKey &v)
{
    RenderKey::Features f = { };

    if(v.shadows()) f |= RenderKey::Feature::Shadows;
    if(v.diffuse()) f |= RenderKey::Feature::Diffuse;

    return f;
}

}

RenderKey::RenderKey() : s(false), d(nullptr), f({ })
{
}

RenderKey::RenderKey(bool shadows, const Gx::Texture *diffuse) : s(shadows), d(diffuse), f(makeFeatures(*this))
{
}

bool operator==(const RenderKey &a, const RenderKey &b)
{
    return a.shadows() == b.shadows() && a.diffuse() == b.diffuse();
}

bool operator!=(const RenderKey &a, const RenderKey &b)
{
    return a.shadows() != b.shadows() || a.diffuse() != b.diffuse();
}

