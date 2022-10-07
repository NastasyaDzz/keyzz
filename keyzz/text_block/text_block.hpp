#ifndef KEYZZ_TEXT_BLOCK_HPP_INCLUDED
#define KEYZZ_TEXT_BLOCK_HPP_INCLUDED

#include <optional>
#include <string>
#include <ncurses.h>
#include <minunity/game_object.hpp>

namespace keyzz {

class TextBlock : public minunity::GameObject {
public:
    TextBlock(int x, int y);
    void destroy() override;
    void render_layer(minunity::Layer layer) override;
    void redraw() override;
    void set_active(bool active) override;
    void start_lap(std::wstring lap_text, std::optional<std::wstring> next_lap_text,
            int lap_index, int laps_count);
    bool input(const int key);
    bool get_race_finished();
    bool get_lap_finished();
private:
    int x_, y_;
    WINDOW* win_ = nullptr;
    WINDOW* win_next_lap_ = nullptr;
    std::wstring lap_text_;
    std::optional<std::wstring> next_lap_text_;
    int lap_index_ = 0;
    int laps_count_ = 0;
    int position_ = 0;
    bool lap_finished_ = false;
    bool race_finished_ = false;
    bool has_error_ = false;
    void draw_lap_text();
    void draw_next_lap_text();
    void set_cursor(int position, bool show_error);
    void clear_cursor(int position);
    void clear();
    bool redraw_flag_ = false;
};

}

#endif // KEYZZ_TEXT_BLOCK_HPP_INCLUDED

