#include "DebugText.h"

#include "graphics/Graphics.h"

#include <GxMaths/GxColor.h>

#include <GxGraphics/GxFont.h>
#include <GxGraphics/GxFontMetrics.h>

namespace
{

Graphics::Handle<Gx::Font> font;

}

void DebugText::acquire(Graphics &graphics)
{
    font = graphics.resources.add(new Gx::Font(graphics.device, { 14, "Consolas", { } }));
}

void DebugText::release()
{
    font = { };
}

int DebugText::width(const std::string &text)
{
    return Gx::FontMetrics(*font).width(text);
}

int DebugText::height()
{
    return Gx::FontMetrics(*font).height();
}

void DebugText::draw(int x, int y, const std::string &text, const Gx::Color &color)
{
    font->draw(x, y, text, color);
}
