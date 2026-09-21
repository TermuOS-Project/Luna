#include "gfx.hpp"
#include "theme.hpp"
#include "window.hpp"
#include "events.hpp"

extern "C" {
#include <input.h>
#include <unistd.h>
}

static void composite(Gfx &g, Window &win)
{
    g.fill_rect(0, 0, g.width(), g.height(), Theme::desktop);
    if (win.visible)
        win.paint_tree(g);
    win.dirty = false;
}

extern "C" int main(void)
{
    Gfx g;
    g.init_from_fb();
    if (g.width() <= 0) {
        write(1, "luna: no fb\n", 12);
        return 1;
    }

    Window win;
    win.x = 120;
    win.y = 90;
    win.w = 400;
    win.h = 260;
    win.title = "Luna";
    win.visible = true;

    composite(g, win);

    uint8_t prev_buttons = 0;
    int mx = 0, my = 0;

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

            if (win.visible)
                win.on_event(ev);

            if (win.dirty)
                composite(g, win);

            /* cursor on top after composite */
            if (mx >= 0 && my >= 0 && mx < g.width() && my < g.height())
                g.put_pixel(mx, my, 0xFFFFFFFFu);
        }

        if (kbd_haschar()) {
            int c = kbd_getchar();
            if (c == 27)
                break;
            if (win.visible) {
                Event kev{};
                kev.type = EventType::KeyDown;
                kev.key = (char)c;
                kev.x = mx;
                kev.y = my;
                win.on_event(kev);
                if (win.dirty)
                    composite(g, win);
            }
        }
    }

    write(1, "luna: exit\n", 11);
    return 0;
}
