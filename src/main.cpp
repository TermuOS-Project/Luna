#include "gfx.hpp"
#include "theme.hpp"
#include "window.hpp"
#include "events.hpp"
#include "cursor.hpp"
#include "wm.hpp"
#include "desktop.hpp"
#include "taskbar.hpp"

extern "C" {
#include <input.h>
#include <unistd.h>
}

static void composite(Gfx &g, Desktop &desk, Wm &wm, Taskbar &bar,
                      Cursor &cur, int mx, int my)
{
    desk.w = g.width();
    desk.h = g.height();
    bar.layout(g.width(), g.height());

    desk.paint(g);
    wm.paint_all(g);
    bar.paint(g);

    desk.dirty = bar.dirty = false;
    wm.clear_dirty();

    if (mx >= 0 && my >= 0 && mx < g.width() && my < g.height())
        cur.draw(g, mx, my);
}

extern "C" int main(void)
{
    Gfx g;
    g.init_from_fb();
    if (g.width() <= 0) {
        write(1, "luna: no fb\n", 12);
        return 1;
    }
    mouse_set_bounds(g.width(), g.height());

    Wm wm;

    Window a, b;
    a.x = 80;
    a.y = 60;
    a.w = 360;
    a.h = 240;
    a.title = "One";
    a.visible = true;

    b.x = 220;
    b.y = 120;
    b.w = 360;
    b.h = 240;
    b.title = "Two";
    b.visible = true;

    Desktop desk;
    Taskbar bar;
    bar.wm = &wm;

    wm.add(&a);
    wm.add(&b);

    Cursor cursor;
    int mx = 100, my = 100;
    int ox = mx, oy = my;
    uint8_t prev_buttons = 0;

    composite(g, desk, wm, bar, cursor, mx, my);

    for (;;) {
        mouse_state m;
        if (mouse_get_state(&m) == 0) {
            mx = m.x;
            my = m.y;

            Event ev{};
            ev.x = m.x;
            ev.y = m.y;
            ev.buttons = m.buttons;

            if ((m.buttons & 1) && !(prev_buttons & 1))
                ev.type = EventType::MouseDown;
            else if (!(m.buttons & 1) && (prev_buttons & 1))
                ev.type = EventType::MouseUp;
            else
                ev.type = EventType::MouseMove;

            prev_buttons = m.buttons;

            if (ev.type == EventType::MouseDown) {
                if (bar.contains_screen(ev.x, ev.y)) {
                    bar.on_event(ev);
                } else if (Window *hit = wm.hit(ev.x, ev.y)) {
                    wm.raise(hit);
                    hit->on_event(ev);
                    bar.mark_dirty();
                } else {
                    desk.on_event(ev);
                }
            } else {
                for (int i = 0; i < wm.count; i++)
                    if (wm.stack[i] && wm.stack[i]->visible)
                        wm.stack[i]->on_event(ev);
                bar.on_event(ev);
            }

            bool need = desk.dirty || bar.dirty || wm.any_dirty() || mx != ox || my != oy;
            if (need) {
                composite(g, desk, wm, bar, cursor, mx, my);
                ox = mx; oy = my;
            }
        }

        if (kbd_haschar()) {
            int c = kbd_getchar();
            if (c == 27)
                break;
        }
    }

    write(1, "luna: exit\n", 11);
    return 0;
}
