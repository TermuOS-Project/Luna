#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include <syscall.h>

struct fb_info_user {
    unsigned long width, height, pitch;
    unsigned int bpp;
};

struct mouse_state {
    int32_t x, y;
    uint8_t buttons;
    int8_t dx, dy;
};

static void putpixel(uint64_t x, uint64_t y, uint32_t c,
                     uint64_t w, uint64_t h)
{
    if (x >= w || y >= h)
        return;
    __syscall3(SYS_FB_PUTPIXEL, (long)x, (long)y, (long)c);
}

int main(void)
{
    struct fb_info_user info;
    struct mouse_state m;
    int ox = -1, oy = -1;
    const char *hello = "Luna: move mouse, Esc to exit\n";

    if (__syscall1(SYS_FB_INFO, (long)&info) != 0) {
        write(1, "luna: fb_info failed\n", 21);
        return 1;
    }

    __syscall1(SYS_FB_CLEAR, (long)0xFF1A1A2E);
    write(1, hello, strlen(hello));

    for (;;) {
        if (__syscall1(SYS_MOUSE_GET_STATE, (long)&m) == 0) {
            if (m.x >= 0 && m.y >= 0 &&
                (uint64_t)m.x < info.width &&
                (uint64_t)m.y < info.height &&
                (m.x != ox || m.y != oy)) {
                if (ox >= 0 && oy >= 0)
                    putpixel((uint64_t)ox, (uint64_t)oy, 0xFF1A1A2E,
                             info.width, info.height);
                putpixel((uint64_t)m.x, (uint64_t)m.y, 0xFFFFFFFF,
                         info.width, info.height);
                ox = m.x;
                oy = m.y;
            }
        }

        if (__syscall0(SYS_KBD_HASCHAR)) {
            long c = __syscall0(SYS_KBD_GETCHAR);
            if (c == 27)
                break;
        }

        __syscall0(SYS_YIELD);
    }

    __syscall1(SYS_FB_CLEAR, (long)0xFF000000);
    write(1, "Luna: exit\n", 11);
    return 0;
}