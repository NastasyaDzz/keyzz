#ifndef SPRITE_HPP_INCLUDED
#define SPRITE_HPP_INCLUDED

#include <vector>

namespace minunity {

class Sprite {
public:
    Sprite(int width, int height, int frames) : width_(width), height_(height), frames_(frames) {};
    ~Sprite();
    int get_width() { return width_; }
    int get_height() { return height_; }
    int get_frames() { return frames_; }
    
    int get_frame_index();
    void set_frame_index(int value);
    const wchar_t* get_row(int index);

    void load(const wchar_t *ch);
    
    bool get_modified() { return is_modified_; }
    void clear_modified() { is_modified_ = false; }
    void repaint() { is_modified_ = true; }
private:
    int width_;
    int height_;
    int frames_;
    std::vector<const wchar_t *> rows_;
    void clear();
    int frame_index_ = 0;
    int previous_frame_index_ = -1;
    bool is_modified_ = true;
};

}

#endif // SPRITE_HPP_INCLUDED
