#include "cursor.hpp"

void Cursor::erase(Gfx &g)
{
    if (!saved_)
        return;
    for (int row = 0; row < H; row++)
        for (int col = 0; col < W; col++)
            g.put_pixel(ox_ + col, oy_ + row, under_[row * W + col]);
    saved_ = 0;
}

void Cursor::draw(Gfx &g, int x, int y)
{
    static const char *shape[] = {
        "X          ",
        "XX         ",
        "X.X        ",
        "X..X       ",
        "X...X      ",
        "X....X     ",
        "X.....X    ",
        "X......X   ",
        "X.......X  ",
        "X.....XXXXX",
        "X..X..X    ",
        "X.X X..X   ",
        "XX  X..X   ",
        "     X..X  ",
        "      XX   ",
    };
    static const int rows = 15;

    erase(g);

    ox_ = x;
    oy_ = y;
    for (int row = 0; row < H; row++)
        for (int col = 0; col < W; col++)
            under_[row * W + col] = g.get_pixel(x + col, y + row);
    saved_ = 1;

    for (int row = 0; row < rows; row++) {
        const char *line = shape[row];
        for (int col = 0; line[col] && col < W; col++) {
            if (line[col] == 'X')
                g.put_pixel(x + col, y + row, 0xFF000000u);
            else if (line[col] == '.')
                g.put_pixel(x + col, y + row, 0xFFFFFFFFu);
        }
    }
}
