#include "app.hpp"
#include "../desktop/startmenu/startmenu.hpp"

extern "C" {
#include <unistd.h>
}

static void trampoline(void *user)
{
    auto *app = (LunaApp *)user;
    if (app && app->open)
        app->open(app->user);
}

// openers declarations
void app_about_open(void *user);
void app_terminal_open(void *user);

static void launch_about(void *user)
{
    (void)user;
    long pid = spawn("/bin/about.tsys");
    if (pid < 0) {
        return;
    }
}

static LunaApp g_apps[] = {
    {"about", "About Luna", "Apps", launch_about, nullptr},
    {"explorer", "File Explorer", "Apps", app_explorer_open, nullptr},
    {"browser", "Browser", "Network", app_browser_open, nullptr},
    {"settings", "Settings", "System", app_settings_open, nullptr},
};

void luna_apps_register_menu(StartMenu &menu)
{
    for (unsigned i = 0; i < sizeof(g_apps) / sizeof(g_apps[0]); i++)
        menu.add_item(g_apps[i].name, g_apps[i].category,
                      trampoline, &g_apps[i]);
}
