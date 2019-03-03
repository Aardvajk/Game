#include "DebugText.h"

#include "graphics/Graphics.h"

#include <GxGraphics/GxFont.h>

namespace
{

std::string curr;
Graphics::Handle<Gx::Font> font;

}

DebugText::~DebugText()
{
    curr = s.str();
}


void DebugText::init(Graphics &graphics)
{
    font = graphics.resources.add(new Gx::Font(graphics.device, { 12, "Arial", Gx::Font::Flag::Bold }));
}

void DebugText::release()
{
    font = { };
}

void DebugText::clear()
{
    curr = { };
}

void DebugText::render(Graphics &graphics)
{
    if(!curr.empty())
    {
        font->draw(2, 754, curr, Gx::Color(1, 1, 1));
    }
}
