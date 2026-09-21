#include "gfx.hpp"
#include "theme.hpp"
#include "window.hpp"

extern "C" {
#include <input.h>
#include <unistd.h>
#include <string.h>
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
    win.x = 120; win.y = 90; win.w = 400; win.h = 260;
    win.title = "Luna";
    win.visible = true;

    g.fill_rect(0, 0, g.width(), g.height(), Theme::desktop);
    win.paint(g);

    int ox = -1, oy = -1;
    const uint32_t bg = Theme::desktop;

    for (;;) {
        mouse_state m;
        if (mouse_get_state(&m) == 0 &&
            m.x >= 0 && m.y >= 0 &&
            m.x < g.width() && m.y < g.height() &&
            (m.x != ox || m.y != oy)) {
            if (ox >= 0 && oy >= 0)
                g.put_pixel(ox, oy, bg);
            g.put_pixel(m.x, m.y, 0xFFFFFFFFu);
            ox = m.x;
            oy = m.y;
        }

        if (kbd_haschar()) {
            int c = kbd_getchar();
            if (c == 27)
                break;
        }
    }

    g.fill_rect(0, 0, g.width(), g.height(), 0xFF000000u);
    write(1, "luna: exit\n", 11);
    return 0;
}
