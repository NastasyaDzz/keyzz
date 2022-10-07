#ifndef ANIMATION_COMPONENT_HPP_INCLUDED
#define ANIMATION_COMPONENT_HPP_INCLUDED

#include "component.hpp"
#include "sprite.hpp"

#include <cstdint>
#include <vector>

namespace minunity {

class GameObject;

class AnimationComponent;

class IAnimationStopped {
public:
    virtual void on_animation_stopped(int animation_id) = 0;
};

class IBeforeAnimationFrame {
public:
    virtual void before_animation_frame(int animation_id, int frame_index) = 0;
};

class AnimationComponent : public Component {
public:
    void init(Sprite *sprite, uint32_t duration, bool looped, const std::vector<int> frames);
    void update() override;

    void play(IAnimationStopped* animation_stopped = nullptr, IBeforeAnimationFrame* before_animation_frame = nullptr);
    void pause();
    void stop();
private:
    bool is_initialized_ = false;
    Sprite *sprite_ = nullptr;
    std::vector<int> frames_;
    uint32_t frame_duration_;
    bool looped_;
    int frames_count_;
    uint32_t frame_index_;
    IAnimationStopped* animation_stopped_ = nullptr;
    IBeforeAnimationFrame* before_animation_frame_ = nullptr;
    bool is_playing_;
    long next_frame_time_;
    void set_frame(int index);
};

}

#endif // ANIMATION_COMPONENT_HPP_INCLUDED
