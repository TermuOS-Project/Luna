#pragma once
#include "widget.hpp"
#include "theme.hpp"

class Desktop : public Widget
{
public:
    uint32_t wallpaper = Theme::desktop;

    void paint(Gfx &g) override
    {
        g.fill_rect(0, 0, w, h, wallpaper);
    }
};
