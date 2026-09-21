#pragma once
#include "window.hpp"

class Wm
{
public:
    static constexpr int kMax = 16;

    Window *stack[kMax]{};
    int count = 0;

    void add(Window *w)
    {
        if (!w || count >= kMax)
            return;
        stack[count++] = w;
        w->visible = true;
        w->mark_dirty();
    }

    void raise(Window *w)
    {
        if (!w)
            return;
        int i;
        for (i = 0; i < count; i++)
            if (stack[i] == w)
                break;
        if (i >= count)
            return;
        for (; i < count - 1; i++)
            stack[i] = stack[i + 1];
        stack[count - 1] = w;
        w->mark_dirty();
    }

    Window *hit(int x, int y)
    {
        for (int i = count - 1; i >= 0; i--)
        {
            Window *w = stack[i];
            if (w && w->visible && w->contains_screen(x, y))
                return w;
        }
        return nullptr;
    }

    void paint_all(Gfx &g)
    {
        for (int i = 0; i < count; i++)
            if (stack[i] && stack[i]->visible)
                stack[i]->paint_tree(g);
    }

    bool any_dirty() const
    {
        for (int i = 0; i < count; i++)
            if (stack[i] && stack[i]->dirty)
                return true;
        return false;
    }

    void clear_dirty()
    {
        for (int i = 0; i < count; i++)
            if (stack[i])
                stack[i]->dirty = false;
    }
};
