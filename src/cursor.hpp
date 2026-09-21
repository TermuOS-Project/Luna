#pragma once
#include "gfx.hpp"
#include <stdint.h>

class Cursor
{
public:
    static constexpr int W = 12;
    static constexpr int H = 18;

    void erase(Gfx &g);
    void draw(Gfx &g, int x, int y);

private:
    int ox_ = 0, oy_ = 0;
    int saved_ = 0;
    uint32_t under_[W * H];
};
