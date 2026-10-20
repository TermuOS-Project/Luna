#include "app.hpp"
#include "../desktop/wm.hpp"
#include "../widgets/window.hpp"
#include "../widgets/label.hpp"
#include "../widgets/button.hpp"
#include "../widgets/gfx.hpp"
#include "../theme.hpp"

extern "C" {
#include <http.h>
}

static Window br_win;
static Label br_status;
static Button br_go;
static bool br_ready;

static char page_buf[8192];
static char body_store[8192];
static const char *body_view = "";

class PageView : public Widget
{
public:
    const char *text = "";

    void paint(Gfx &g) override
    {
        int sx, sy;
        screen_pos(sx, sy);
        g.fill_rect(sx, sy, w, h, Theme::client);

        if (!text)
            return;

        int x = sx + 4;
        int y = sy + 4;
        int line_h = 14;
        int max_y = sy + h - line_h;
        int col = 0;
        const int cols = (w - 8) / 8;
        if (cols < 8)
            return;

        char line[128];
        int li = 0;

        auto flush = [&]() {
            line[li] = '\0';
            if (y <= max_y)
                g.draw_text(x, y, line, Theme::text, Theme::client);
            y += line_h;
            li = 0;
            col = 0;
        };

        for (const char *p = text; *p && y <= max_y; p++) {
            if (*p == '\r')
                continue;
            if (*p == '\n') {
                flush();
                continue;
            }
            if (col >= cols || li + 1 >= (int)sizeof(line))
                flush();
            if (y > max_y)
                break;
            line[li++] = *p;
            col++;
        }
        if (li > 0 && y <= max_y)
            flush();
    }
};

static PageView br_page;
static bool br_busy = false;

static void browser_go(void *user)
{
    (void)user;
    if (br_busy)
        return;
    br_busy = true;
    br_status.text = "Loading...";
    br_win.mark_dirty();

    uint8_t ip[4] = {10, 0, 2, 2};
    uint16_t port = 18080;
    const char *path = "/";

    br_status.text = "Loading...";
    br_win.mark_dirty();

    int n = http_get(ip, port, path, page_buf, sizeof(page_buf));
    if (n < 0) {
        br_status.text = "Failed";
        body_view = "";
        br_page.text = body_view;
        br_win.mark_dirty();
        return;
    }

    const char *body = http_body(page_buf);
    size_t i = 0;
    while (body[i] && i + 1 < sizeof(body_store)) {
        body_store[i] = body[i];
        i++;
    }
    body_store[i] = '\0';
    body_view = body_store;
    br_page.text = body_view;
    br_status.text = "OK - http://10.0.2.2:18080/";
    br_busy = false;
    br_win.mark_dirty();
}

void app_browser_open(void *user)
{
    (void)user;

    if (!br_ready) {
        br_win.x = 60;
        br_win.y = 40;
        br_win.w = 520;
        br_win.h = 360;
        br_win.title = "Browser";
        br_win.visible = false;

        br_status.x = 12;
        br_status.y = 36;
        br_status.w = 400;
        br_status.h = 18;
        br_status.text = "http://10.0.2.2:18080/  (hardcoded)";
        br_win.add(&br_status);

        br_go.x = 420;
        br_go.y = 34;
        br_go.w = 80;
        br_go.h = 24;
        br_go.text = "Go";
        br_go.on_click = browser_go;
        br_win.add(&br_go);

        br_page.x = 12;
        br_page.y = 64;
        br_page.w = 496;
        br_page.h = 280;
        br_page.text = "";
        br_win.add(&br_page);

        br_ready = true;
        if (g_luna_wm)
            g_luna_wm->add(&br_win);
    }

    br_win.visible = true;
    if (g_luna_wm)
        g_luna_wm->raise(&br_win);
    br_win.mark_dirty();
}