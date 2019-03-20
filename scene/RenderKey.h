#ifndef RENDERKEY_H
#define RENDERKEY_H

#include <pcx/flags.h>

#include <functional>

namespace Gx
{

class Texture;

}

class RenderKey
{
public:
    enum class Feature
    {
        Shadows = 1,
        Diffuse = 2
    };

    using Features = pcx::flags<Feature>;

    RenderKey();
    RenderKey(bool shadows, const Gx::Texture *diffuse);

    bool shadows() const { return s; }
    const Gx::Texture *diffuse() const { return d; }
    Features features() const { return f; }

private:
    bool s;
    const Gx::Texture *d;

    Features f;
};

bool operator==(const RenderKey &a, const RenderKey &b);
bool operator!=(const RenderKey &a, const RenderKey &b);

template<> struct pcx_is_flag_enum<RenderKey::Feature> : std::true_type { };

#endif // RENDERKEY_H
