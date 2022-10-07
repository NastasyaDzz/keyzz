#ifndef ENGINE_HPP_INCLUDED
#define ENGINE_HPP_INCLUDED

#include <logger/easylogging++.h>

#include <memory>
#include <vector>

#include "collider_component.hpp"
#include "collider_groups.hpp"
#include "scene.hpp"
#include "time.hpp"

namespace minunity {

struct ColorInfo {
    const bool HAS_COLORS;
    const bool CAN_CHANGE_COLOR;
    const int COLORS_COUNT;
    const int COLOR_PAIRS_COUNT;

    ColorInfo(bool has_colors, bool can_change_color, int colors_count, int color_pairs_count) :
        HAS_COLORS(has_colors),
        CAN_CHANGE_COLOR(can_change_color),
        COLORS_COUNT(colors_count),
        COLOR_PAIRS_COUNT(color_pairs_count)
    {}

    // ColorInfo& operator=(const ColorInfo&) = delete;
};

class Engine {
public:
    static Engine *get_instance();
    
    el::base::type::StoragePointer get_log_storage();

    void start();

    void init_color();
    ColorInfo& get_color_info();

    void run();
    void run_once();
    void stop();
    void terminate();
    Scene* get_scene();
    void set_scene(Scene *scene);
    ColliderGroups& get_collider_groups();
    Time *get_time();
    void redraw();
private:
    static const int DELAY_MICROSECONDS = 5;
    bool is_started_ = false;
    bool is_running_ = false;
    Scene *scene_ = nullptr;
    ColliderGroups collider_groups_;
    Time *game_time = new Time(50);
    void run_once_impl();
    std::shared_ptr<ColorInfo> color_info_ = std::shared_ptr<ColorInfo>(new ColorInfo(false, false, 0, 0));
};

} // namespace minunity

#endif // ENGINE_HPP_INCLUDED
