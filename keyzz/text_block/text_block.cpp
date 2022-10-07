// Copyright 2021 DmitryDzz

#include "text_block.hpp"

#include <logger/easylogging++.h>

#include <minunity/graph.hpp>

#include "../app_manager.hpp"
#include "../tracks/race.hpp"

using keyzz::TextBlock;
using minunity::Graph;
using minunity::Layer;

TextBlock::TextBlock(int x, int y) : x_(x), y_(y), lap_text_(L""), next_lap_text_(L"") {
    win_ = newwin(3, Race::LAP_MAX_SIZE, y_, x_);
    win_next_lap_ = newwin(3, Race::NEXT_LAP_SIZE, y_, x_ + Race::LAP_MAX_SIZE);
}

void TextBlock::destroy() {
    if (win_ != nullptr) {
        delwin(win_);
        win_ = nullptr;
    }
    if (win_next_lap_ != nullptr) {
        delwin(win_next_lap_);
        win_next_lap_ = nullptr;
    }
}

void TextBlock::render_layer(Layer layer) {
    GameObject::render_layer(layer);
    if (redraw_flag_) {
        redraw_flag_ = false;
        clear();
        set_cursor(position_, has_error_);
        draw_lap_text();
        draw_next_lap_text();
    }
}

void TextBlock::redraw() {
    if (!get_active()) return;
    redraw_flag_ = true;
}

void TextBlock::set_active(bool active) {
    GameObject::set_active(active);
    if (!active)
        clear();
}

void TextBlock::start_lap(std::wstring lap_text, std::optional<std::wstring> next_lap_text,
        int lap_index, int laps_count) {
    clear();
    lap_text_ = lap_text;
    next_lap_text_ = next_lap_text;
    lap_index_ = lap_index;
    laps_count_ = laps_count;
    has_error_ = false;
    set_cursor(0, has_error_);
    draw_lap_text();
    draw_next_lap_text();
    position_ = 0;
    race_finished_ = false;
    lap_finished_ = false;
//    LOG(INFO) << "[TextBlock] " << position_;
}

void TextBlock::draw_lap_text() {
    if (!win_) return;
    AppManager& app_manager = AppManager::get_instance();
    AppManager::ColorPairIndexes* colors = app_manager.get_color_pair_indexes();
    if (colors != nullptr)
        wattron(win_, COLOR_PAIR(colors->PRIMARY));
    Graph::win_draw(win_, lap_text_.c_str(), 0, 1);
    if (colors != nullptr)
        wattroff(win_, COLOR_PAIR(colors->PRIMARY));
    wrefresh(win_);
}

void TextBlock::draw_next_lap_text() {
    if (!win_next_lap_) return;
    if (next_lap_text_) {
        AppManager& app_manager = AppManager::get_instance();
        AppManager::ColorPairIndexes* colors = app_manager.get_color_pair_indexes();
        if (colors != nullptr)
            wattron(win_next_lap_, COLOR_PAIR(colors->SECONDARY));
        Graph::win_draw(win_next_lap_, L"Next lap", 0, 0);
        Graph::win_draw(win_next_lap_, next_lap_text_.value().c_str(), 0, 1);
        Graph::win_draw(win_next_lap_, L"‾‾‾‾‾‾‾‾", 0, 2);
        if (colors != nullptr)
            wattroff(win_next_lap_, COLOR_PAIR(colors->SECONDARY));
    } else {
        werase(win_next_lap_);
    }
    wrefresh(win_next_lap_);
}

void TextBlock::set_cursor(int position, bool show_error) {
    if (!win_) return;
    AppManager& app_manager = AppManager::get_instance();
    AppManager::ColorPairIndexes* colors = app_manager.get_color_pair_indexes();
    if (colors != nullptr)
        wattron(win_, COLOR_PAIR(colors->PRIMARY));
    Graph::win_draw(win_, show_error ? L"E" : L"_", position, 0);
    Graph::win_draw(win_, L"‾", position, 2);
    if (colors != nullptr)
        wattroff(win_, COLOR_PAIR(app_manager.get_color_pair_indexes()->PRIMARY));
    wrefresh(win_);
}

void TextBlock::clear_cursor(int position) {
    if (!win_) return;
    AppManager& app_manager = AppManager::get_instance();
    AppManager::ColorPairIndexes* colors = app_manager.get_color_pair_indexes();
    if (colors != nullptr)
        wattron(win_, COLOR_PAIR(colors->PRIMARY));
    Graph::win_draw(win_, L" ", position, 0);
    Graph::win_draw(win_, L" ", position, 2);
    if (colors != nullptr)
        wattroff(win_, COLOR_PAIR(app_manager.get_color_pair_indexes()->PRIMARY));
    wrefresh(win_);
}

void TextBlock::clear() {
    if (win_) {
        werase(win_);
        wrefresh(win_);
    }
    if (win_next_lap_) {
        werase(win_next_lap_);
        wrefresh(win_next_lap_);
    }
}

bool TextBlock::input(const int key) {
    AppManager::Settings* settings = AppManager::get_instance().get_settings();
    int visible_key_code;
    if (key == 0x09)
        visible_key_code = settings->TAB_CODE;
    else if (key == 0x0A)
        visible_key_code = settings->RETURN_CODE;
    else
        visible_key_code = key;

    if (lap_text_[position_] == visible_key_code) {
        clear_cursor(position_);
        position_++;
        if (static_cast<uint64_t>(position_) < lap_text_.size()) {
            has_error_ = false;
            set_cursor(position_, has_error_);
        } else {
            lap_finished_ = true;
            race_finished_ = lap_index_ == laps_count_ - 1;
        }
        return true;
    }
    has_error_ = true;
    set_cursor(position_, has_error_);
    return false;
}

bool TextBlock::get_race_finished() {
    return race_finished_;
}

bool TextBlock::get_lap_finished() {
    return lap_finished_;
}
