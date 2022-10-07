// Copyright 2021 DmitryDzz

#include "my_runner.hpp"

#include <logger/easylogging++.h>

#include <minunity/engine.hpp>

#include "../tracks/race.hpp"

using minunity::AnimationComponent;
using minunity::AnimatorComponent;
using minunity::Engine;
using minunity::Layer;
using minunity::Sprite;

using keyzz::BaseRunner;
using keyzz::MyRunner;
using keyzz::Race;
using keyzz::Track;

MyRunner::MyRunner(int x, int y, std::shared_ptr<Track> track_record)
    : BaseRunner(x, y, track_record) {
    idle_animation_ = std::dynamic_pointer_cast<AnimationComponent>(add_component(new AnimationComponent()));
    finish_animation_ = std::dynamic_pointer_cast<AnimationComponent>(add_component(new AnimationComponent()));
    penalty_animation_ = std::dynamic_pointer_cast<AnimationComponent>(add_component(new AnimationComponent()));
    animator_ = std::dynamic_pointer_cast<AnimatorComponent>(add_component(new AnimatorComponent()));
}

void MyRunner::awake() {
    BaseRunner::awake();

    idle_animation_->init(get_sprite(), 350, true, { 0, 1 });
    finish_animation_->init(get_sprite(), 0, false, { 0 });
    penalty_animation_->init(get_sprite(), 1000, false, { 2, 3, 4, 5, 6, 7, 8, 9 });
    animator_->add_animation(idle_animation_);
    animator_->add_animation(finish_animation_);
    animator_->add_animation(penalty_animation_);
}

void MyRunner::render_layer(Layer layer) {
    if (is_destroyed() || get_sprite() == nullptr) return;
    BaseRunner::render_layer(layer);
}

Sprite* MyRunner::create_sprite() {
    Sprite *result = new Sprite(3, 1, 10);
    result->load(L"(U)(u)│▁││▂││▃││▄││▅││▆││▇││█│");
    return result;
}

void MyRunner::start_lap(int lap_index, int laps_count) {
    BaseRunner::start_lap(lap_index, laps_count);

    set_x(get_start_x());
    if (lap_index == 0) {
        track_->clear_slices();
        track_->add_slice(0, 0);
        animator_->play(idle_animation_->get_id());
    }
    if (!race_start_time_)
        race_start_time_ = Engine::get_instance()->get_time()->get_time();
}

void MyRunner::move_one_step() {
    if (race_start_time_ && !in_error_delay_) {
        int x = get_x() + 1;
        int slice_x = get_lap_index() * Race::LAP_MAX_SIZE + x - get_start_x();
        uint32_t t = Engine::get_instance()->get_time()->get_time() - race_start_time_.value();
        track_->add_slice(slice_x, t);
        // LOG(INFO) << "x: " << slice_x << ", t: " << t;
        set_x(x);
    }
}

void MyRunner::finish() {
    animator_->play(finish_animation_->get_id());
    track_->set_finished(true);
    race_start_time_ = std::nullopt;
}

void MyRunner::start_error_delay() {
    in_error_delay_ = true;
    animator_->play(penalty_animation_->get_id(), this);
}

bool MyRunner::in_error_delay() {
    return in_error_delay_;
}

void MyRunner::on_animation_stopped(int animation_id) {
    if (animation_id == penalty_animation_->get_id()) {
        in_error_delay_ = false;
        animator_->play(idle_animation_->get_id());
    }
}
