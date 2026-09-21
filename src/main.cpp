#include <unistd.h>
#include <string.h>
#include <fb.h>
#include <input.h>

int main(void)
{
    struct fb_info info;
    struct mouse_state m;
    int ox = -1, oy = -1;

    if (fb_info(&info) != 0)
    {
        write(1, "luna: fb_info failed\n", 21);
        return 1;
    }

    fb_clear(0xFF1A1A2E);
    write(1, "Luna: move mouse, Esc to exit\n", 30);

    for (;;) {
        if (mouse_get_state(&m) == 0 &&
            m.x >= 0 && m.y >= 0 &&
            (uint64_t)m.x < info.width &&
            (uint64_t)m.y < info.height &&
            (m.x != ox || m.y != oy)) {
            if (ox >= 0 && oy >= 0)
                fb_putpixel((uint32_t)ox, (uint32_t)oy, 0xFF1A1A2E);
            fb_putpixel((uint32_t)m.x, (uint32_t)m.y, 0xFFFFFFFF);
            ox = m.x;
            oy = m.y;
        }

        if (kbd_haschar()) {
            int c = kbd_getchar();
            if (c == 27)
                break;
        }
    }

    fb_clear(0xFF000000);
    write(1, "Luna: exit\n", 11);
    return 0;
}
