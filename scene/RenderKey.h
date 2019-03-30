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
        Diffuse = 2,
        Normal = 4
    };

    using Features = pcx::flags<Feature>;

    RenderKey();
    RenderKey(bool shadows, const Gx::Texture *diffuse, const Gx::Texture *normal);

    bool shadows() const { return s; }
    const Gx::Texture *diffuse() const { return d; }
    const Gx::Texture *normal() const { return n; }
    Features features() const { return f; }

private:
    bool s;
    const Gx::Texture *d;
    const Gx::Texture *n;

    Features f;
};

bool operator==(const RenderKey &a, const RenderKey &b);
bool operator!=(const RenderKey &a, const RenderKey &b);

template<> struct pcx_is_flag_enum<RenderKey::Feature> : std::true_type { };

#endif // RENDERKEY_H
