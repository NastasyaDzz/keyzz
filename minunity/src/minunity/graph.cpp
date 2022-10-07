// Copyright 2020 DmitryDzz

#include "graph.hpp"

#include <iostream>

using minunity::Graph;

void Graph::draw(const wchar_t *ch, int x, int y) {
    mvaddwstr(y, x, ch);
}

void Graph::draw(wchar_t *ch, int x, int y) {
    mvaddwstr(y, x, ch);
}

void Graph::win_draw(WINDOW* win, const wchar_t *ch, int x, int y) {
    mvwaddwstr(win, y, x, ch);
}

void Graph::win_draw(WINDOW* win, wchar_t *ch, int x, int y) {
    mvwaddwstr(win, y, x, ch);
}

/*void Graph::draw_something() {
    start_color();
    init_pair(1, COLOR_CYAN, COLOR_BLACK);
    
    printw("Hello ncurses!\n");
    
    attron(COLOR_PAIR(1));
    //wchar_t playerChar = L'\u2588';
    const char* idle = "█";
    printw(idle);
    // printw(idle + "\n");
    // const char* jump0 = "▄";
    // const char* jump1 = "▀";
    // const char* jump2 = "█";
    // printw(jump0 + "\n");
    // printw(jump1 + "\n");
    // printw(jump2 + "\n");
    // printw("%c\n", (char)219);
    printw("\n");
    for (int i = 0; i < 5; i++) {
          printw("%c %lc\n", '0' + i, L'０' + i);
    }
    attroff(COLOR_PAIR(1));
}*/

void Graph::draw_vertical_line(const wchar_t *ch, int x, int y, int length) {
    int end = y + length;
    for (int yy = y; yy < end; yy++) {
        mvaddwstr(yy, x, ch);
    }
}

void Graph::draw_horizontal_line(const wchar_t *ch, int x, int y, int length) {
    int end = x + length;
    for (int xx = x; xx < end; xx++) {
        mvaddwstr(y, xx, ch);
    }
}

void Graph::draw_rect(const wchar_t *ch, Rect &rect) {
    for (int yy = rect.y; yy < rect.y + rect.h; yy++)
        for (int xx = rect.x; xx < rect.x + rect.w; xx++)
            mvaddwstr(yy, xx, ch);
}

void Graph::update() {
    refresh();
}

char Graph::create_pair(int index, int fg_color, int bg_color) {
    init_pair(index, fg_color, bg_color);
    return index;
}

void Graph::change_color(int index, int r, int g, int b) {
    init_color(index, r, g, b);
}
