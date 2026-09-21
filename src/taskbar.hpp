#pragma once
#include "widget.hpp"
#include "theme.hpp"
#include "wm.hpp"

class Taskbar : public Widget
{
public:
    static constexpr int kH = 28;
    Wm *wm = nullptr;

    void layout(int screen_w, int screen_h)
    {
        x = 0;
        y = screen_h - kH;
        w = screen_w;
        h = kH;
    }

    void paint(Gfx &g) override
    {
        g.draw_raised(x, y, w, h);
        /* Start button stub */
        g.draw_raised(x + 2, y + 2, 54, h - 4);
        g.draw_text(x + 8, y + 6, "Start", Theme::text, Theme::face);

        /* simple window buttons */
        int bx = x + 64;
        if (wm) {
            for (int i = 0; i < wm->count; i++) {
                Window *win = wm->stack[i];
                if (!win || !win->visible)
                    continue;
                g.draw_raised(bx, y + 2, 80, h - 4);
                g.draw_text(bx + 6, y + 6,
                            win->title ? win->title : "?",
                            Theme::text, Theme::face);
                bx += 84;
            }
        }
    }

    bool on_event(const Event &e) override
    {
        if (e.type != EventType::MouseDown || !(e.buttons & 1))
            return false;
        if (!contains_screen(e.x, e.y))
            return false;

        /* Start button */
        if (e.x < x + 56) {
            mark_dirty();
            return true;
        }

        int bx = x + 64;
        if (wm) {
            for (int i = 0; i < wm->count; i++) {
                Window *win = wm->stack[i];
                if (!win || !win->visible)
                    continue;
                if (e.x >= bx && e.x < bx + 80) {
                    wm->raise(win);
                    mark_dirty();
                    return true;
                }
                bx += 84;
            }
        }
        return true;
    }
};
