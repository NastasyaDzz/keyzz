// Copyright 2020 DmitryDzz

#include "animation_component.hpp"

#include <logger/easylogging++.h>

#include "engine.hpp"
#include "game_object.hpp"

using minunity::AnimationComponent;

void AnimationComponent::init(Sprite *sprite, uint32_t duration, bool looped, const std::vector<int> frames) {
    is_playing_ = false;
    sprite_ = sprite;
    frames_count_ = frames.size();
    frame_duration_ = frames_count_ == 0 ? 0 : duration / frames_count_;
    looped_ = looped;
    frames_ = frames;
    set_frame(0);
    is_initialized_ = sprite != nullptr;
}

void AnimationComponent::update() {
    if (get_game_object()->is_destroyed()) return;
    if (!is_initialized_ || !is_playing_) return;
    uint32_t time = Engine::get_instance()->get_time()->get_time();
    if (time >= next_frame_time_) {
        next_frame_time_ += frame_duration_;
        if (!looped_ && ((frame_index_ + 1) / frames_count_ > 0))
            stop();
        else
            set_frame((frame_index_ + 1) % frames_count_);
//        sprite_->repaint();
    }
}

void AnimationComponent::play(IAnimationStopped* animation_stopped, IBeforeAnimationFrame* before_animation_frame) {
    if (!is_initialized_) return;
    animation_stopped_ = animation_stopped;
    before_animation_frame_ = before_animation_frame;
    is_playing_ = true;
    set_frame(0);
    next_frame_time_ = Engine::get_instance()->get_time()->get_time() + frame_duration_;
}

void AnimationComponent::pause() {
    if (!is_initialized_) return;
    is_playing_ = false;
}

void AnimationComponent::stop() {
    if (!is_initialized_) return;
    bool was_playing = is_playing_;
    is_playing_ = false;
    if (animation_stopped_ != nullptr && was_playing) {
        animation_stopped_->on_animation_stopped(get_id());
        animation_stopped_ = nullptr;
    }
}

void AnimationComponent::set_frame(int index) {
    if (before_animation_frame_ != nullptr && is_playing_) {
        before_animation_frame_->before_animation_frame(get_id(), index);
    }
    frame_index_ = index;
    sprite_->set_frame_index(frames_[index]);
}
