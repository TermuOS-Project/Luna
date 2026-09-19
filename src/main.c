#include <unistd.h>
#include <string.h>
#include <syscall.h>

struct fb_info_user {
    unsigned long width, height, pitch;
    unsigned int bpp;
};

int main(void)
{
    const char *msg = "Luna phase1: FILL_RECT\n";
    struct fb_info_user info;
    long rc;

    rc = __syscall1(SYS_FB_INFO, (long)&info);
    if (rc != 0) {
        write(1, "luna: fb_info failed\n", 21);
        return 1;
    }

    __syscall1(SYS_FB_CLEAR, (long)0xFF1A1A2E);

    __syscall5(SYS_FB_FILL_RECT, 100, 80, 400, 200, (long)0xFF4060C0);

    write(1, msg, strlen(msg));
    return 0;
}