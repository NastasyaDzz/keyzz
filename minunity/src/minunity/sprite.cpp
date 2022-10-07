// Copyright 2020 DmitryDzz

#include "sprite.hpp"

// #include <logger/easylogging++.h>
#include <cstring>
#include <cwchar>

#include "game_exception.hpp"

using minunity::Sprite;

Sprite::~Sprite() {
    clear();
}

int Sprite::get_frame_index() {
    return frame_index_;
}

void Sprite::set_frame_index(int value) {
    if (value != previous_frame_index_) {
        frame_index_ = value;
        previous_frame_index_ = value;
        is_modified_ = true;
    }
}

const wchar_t* Sprite::get_row(int index) {
    return rows_[frame_index_ * height_ + index];
}

void Sprite::load(const wchar_t *ch) {
    int size = width_ * height_ * frames_;
    if (wcslen(ch) != (uint32_t) size)
        throw new GameException(GameException::WRONG_SPRITE_SIZE, "Wrong sprite size");

    clear();
    int row_offset = 0;
    for (int frame = 0; frame < frames_; frame++) {
        for (int row = 0; row < height_; row++) {
            wchar_t *row_chars = new wchar_t[width_ + 1];
            wcsncpy(row_chars, ch + row_offset, width_);
            row_chars[width_] = '\0';

//            std::wstring row_str(row_chars);
//            LOG(INFO) << row_str << " f" << frame << " r" << row << " o" << row_offset;

            rows_.push_back(row_chars);
            row_offset += width_;
        }
    }

    is_modified_ = true;
    previous_frame_index_ = -1;
    frame_index_ = 0;

    //  for (long unsigned int i = 0; i < rows_.size(); i++) {
    //      mvwaddwstr(stdscr, i, 0, rows_[i]);
    //  }
}

void Sprite::clear() {
    for (const wchar_t *ch : rows_)
        delete[] ch;
    rows_.clear();

    is_modified_ = true;
    previous_frame_index_ = -1;
    frame_index_ = 0;
}
