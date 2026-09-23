#include "gfx.hpp"
#include "../theme.hpp"
#include "../font.hpp"

extern "C" {
#include <fb.h>
#include <stdint.h>
}

void Gfx::init_from_fb()
{
    struct fb_info info{};
    if (fb_info(&info) != 0)
    {
        w_ = h_ = 0;
        return;
    }
    h_ = (int)info.height;
    w_ = (int)info.width;
}

int Gfx::width() const { return w_; }
int Gfx::height() const { return h_; }

void Gfx::fill_rect(int x, int y, int w, int h, uint32_t colour)
{
    if (w <= 0 || h <= 0)
        return;
    fb_fill_rect((uint64_t)x, (uint64_t)y, (uint64_t)w, (uint64_t)h, colour);
}

void Gfx::put_pixel(int x, int y, uint32_t colour)
{
    if (x < 0 || y < 0 || x >= w_ || y >= h_)
        return;
    fb_putpixel((uint64_t)x, (uint64_t)y, colour);
}

uint32_t Gfx::get_pixel(int x, int y)
{
    if (x < 0 || y < 0 || x >= w_ || y >= h_)
        return 0;
    return (uint32_t)fb_getpixel((uint32_t)x, (uint32_t)y);
}

uint32_t Gfx::rgb(uint8_t r, uint8_t g, uint8_t b)
{
    return 0xFF000000u | ((uint32_t)r << 16) | ((uint32_t)g << 8) | b;
}

void Gfx::draw_rect(int x, int y, int w, int h, uint32_t colour)
{
    if (w <= 0 || h <= 0) return;
    fill_rect(x, y, w, 1, colour);
    fill_rect(x, y + h - 1, w, 1, colour);
    fill_rect(x, y, 1, h, colour);
    fill_rect(x + w - 1, y, 1, h, colour);
}

void Gfx::draw_char(int x, int y, char c, uint32_t fg, uint32_t bg)
{
    if (c < 32 || c > 126)
        c = '?';
    const uint8_t *glyph = g_font_8x16[(unsigned char)c - 32];
    for (int row = 0; row < FONT_H; row++)
    {
        uint8_t bits = glyph[row];
        for (int col = 0; col < FONT_W; col++)
        {
            uint32_t colr = (bits & (0x80 >> col)) ? fg : bg;
            put_pixel(x + col, y + row, colr);
        }
    }
}

void Gfx::draw_text(int x, int y, const char *s, uint32_t fg, uint32_t bg)
{
    if (!s)
        return;
    int cx = x;
    for (; *s; ++s)
    {
        if (*s == '\n')
        {
            cx = x;
            y += FONT_H;
            continue;
        }
        draw_char(cx, y, *s, fg, bg);
        cx += FONT_W;
    }
}

void Gfx::blit_rgba(int x, int y, int w, int h, const uint8_t *rgba)
{
    if (!rgba)
        return;
    for (int row = 0; row < h; row++)
    {
        for (int col = 0; col < w; col++)
        {
            const uint8_t *p = rgba + (row * w + col) * 4;
            uint8_t r = p[0], g = p[1], b = p[2], a = p[3];
            if (a < 16)
                continue;
            put_pixel(x + col, y + row, 0xFF000000u | (r << 16) | (g << 8) | b);
        }
    }
}

void Gfx::draw_raised(int x, int y, int w, int h)
{
    fill_rect(x, y, w, h, Theme::face);
    // highlight top/left
    fill_rect(x, y, w, 1, Theme::highlight);
    fill_rect(x, y, 1, h, Theme::highlight);
    // shadow bottom/right
    fill_rect(x, y + h - 1, w, 1, Theme::dkshadow);
    fill_rect(x + w - 1, y, 1, h, Theme::dkshadow);
    fill_rect(x + 1, y + h - 2, w - 2, 1, Theme::shadow);
    fill_rect(x + w - 2, y + 1, 1, h - 2, Theme::shadow);
}

void Gfx::draw_sunken(int x, int y, int w, int h)
{
    fill_rect(x, y, w, h, Theme::client);
    fill_rect(x, y, w, 1, Theme::shadow);
    fill_rect(x, y, 1, h, Theme::shadow);
    fill_rect(x, y + h - 1, w, 1, Theme::highlight);
    fill_rect(x + w - 1, y, 1, h, Theme::highlight);
}
