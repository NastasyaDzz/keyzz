#ifndef GRAPH_HPP_INCLUDED
#define GRAPH_HPP_INCLUDED

#include <ncurses.h>

namespace minunity {

struct Point {
    int x;
    int y;
    Point() : x(0), y(0) {};
    Point(int xx, int yy) : x(xx), y(yy) {};
    void set(Point &p) { x = p.x; y = p.y; }
    void set(int xx, int yy) { x = xx; y = yy; }
};

struct Rect {
    int x;
    int y;
    int w;
    int h;
    Rect() : x(0), y(0), w(1), h(1) {};
    Rect(int xx, int yy, int ww, int hh) : x(xx), y(yy) {
        w = ww < 1 ? 1 : ww;
        h = hh < 1 ? 1 : hh;
    };
    void set(int xx, int yy, int ww, int hh) { x = xx; y = yy; w = ww < 1 ? 1 : ww; h = hh < 1 ? 1 : hh; }
    void set_position(int xx, int yy) { x = xx; y = yy; }
};

class Graph {
public:
    static void draw(const wchar_t *ch, int x, int y);
    static void draw(wchar_t *ch, int x, int y);
    static void win_draw(WINDOW* win, const wchar_t *ch, int x, int y);
    static void win_draw(WINDOW* win, wchar_t *ch, int x, int y);
    static void draw_horizontal_line(const wchar_t *ch, int x, int y, int length);
    static void draw_vertical_line(const wchar_t *ch, int x, int y, int length);
    static void draw_rect(const wchar_t *ch, Rect &rect);
    static void update();

    static char create_pair(int index, int fg_color, int bg_color);
    static void change_color(int index, int r, int g, int b);
};

} // namespace minunity

#endif // GRAPH_HPP_INCLUDED
