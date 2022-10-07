// Copyright 2020 DmitryDzz

#include "input.hpp"

// #include <logger/easylogging++.h>

#include <ncurses.h>

#include <algorithm>
#include <optional>
#include <vector>

#include "engine.hpp"

using minunity::Input;

void Input::start() {
    cbreak();
    noecho();
    nodelay(stdscr, true);
    // halfdelay(5);
    scrollok(stdscr, true);
    keypad(stdscr, true);
}

std::optional<int> last_key = std::nullopt;

void Input::update() {
    last_key = std::nullopt;
    int ch;
    while ((ch = getch()) != ERR) {
        last_key = ch;
    }
//    if (last_key)
//        LOG(INFO) << "[minunity::Input] key=" << last_key.value();
}

bool Input::is_key_pressed(int key) {
    return last_key == key;
}
