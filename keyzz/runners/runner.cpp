// Copyright 2021 DmitryDzz

#include "runner.hpp"

#include <logger/easylogging++.h>

#include <algorithm>

#include <minunity/engine.hpp>

#include "../tracks/race.hpp"
#include "../tracks/track.hpp"

using minunity::Engine;
using minunity::Layer;
using minunity::Sprite;

using keyzz::BaseRunner;
using keyzz::Race;
using keyzz::Runner;
using keyzz::Track;

Runner::Runner(int x, int y, std::shared_ptr<Track> track_record)
    : BaseRunner(x, y, track_record) {
}

void Runner::awake() {
    BaseRunner::awake();
}

void Runner::update() {
    BaseRunner::update();
    if (is_destroyed() || !get_active() || !start_time_) return;

    uint32_t delta_time = Engine::get_instance()->get_time()->get_time() - start_time_.value();
    bool is_previous_lap = false;
    bool is_next_lap = false;
    bool is_finish = false;
    int32_t x = get_start_x() + track_->get_delta_x(delta_time, lap_first_index_, lap_last_index_,
            &is_previous_lap, &is_next_lap, &is_finish);
//    LOG(INFO) << "[Runner] " << track_->get_name() << ": x=" << x << " get_start_x()=" <<
//            get_start_x() << " is_finish=" << is_finish;
    if (get_sprite() != nullptr) {
        if (is_previous_lap) {
            get_sprite()->set_frame_index(1);  // <<
        } else if (is_next_lap) {
            get_sprite()->set_frame_index(2);  // >>
        } else {
            get_sprite()->set_frame_index(is_finish ? 3 : 0);  // (x)
        }
    }
    set_x(x);
    if (is_finish)
        finish_race();
}

void Runner::render_layer(Layer layer) {
    if (is_destroyed() || get_sprite() == nullptr) return;
    BaseRunner::render_layer(layer);
}

void Runner::start_lap(int lap_index, int laps_count) {
    BaseRunner::start_lap(lap_index, laps_count);

    if (!start_time_) {
        start_time_ = Engine::get_instance()->get_time()->get_time();
        // LOG(INFO) << "[Runner] n=" << track_->get_name().c_str() <<
        //     " start_time_=" << start_time_.value();
    }

    lap_first_index_ = lap_index * Race::LAP_MAX_SIZE;
    lap_last_index_ = std::min(lap_first_index_ + Race::LAP_MAX_SIZE - 1, track_->get_size() - 2);

    // LOG(INFO) << "[Runner] n=" << track_->get_name().c_str() <<
    //     " li=" << lap_index << " lc=" << laps_count <<
    //     " lfi=" << lap_first_index_ << " lli=" << lap_last_index_;
}

void Runner::finish_race() {
    start_time_ = std::nullopt;
    track_->set_finished(true);
}

Sprite* Runner::create_sprite() {
    Sprite *result = new Sprite(3, 1, 4);
    std::wstring name = track_->get_name();
    std::wstring upper_name = name;  // upper_name is for the finish.
    std::transform(upper_name.begin(), upper_name.end(), upper_name.begin(), std::towupper);
    std::wstring res = name + L"<< " + L" >>" + upper_name;
    result->load(res.c_str());
    return result;
}
