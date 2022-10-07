// Copyright 2021 DmitryDzz

#include "track.hpp"

#include <logger/easylogging++.h>

#include "race.hpp"

using keyzz::Slice;
using keyzz::Track;
using keyzz::Race;

int32_t s_track_id = 0;

Slice::Slice() : x(0), t(0) {}
Slice::Slice(int32_t x, uint32_t t) : x(x), t(t) {}

Track::Track(Race* race, std::wstring name) : race_(race), name_(name) {
}

int32_t Track::get_id() {
    return ++s_track_id;
}

std::size_t Track::get_size() {
    return slices_.size();
}

void Track::clear_slices() {
    for (auto i = slices_.size(); i-- > 0;)
        delete slices_[i];
    slices_.clear();
}

void Track::add_slice(int32_t x, uint32_t t) {
    slices_.push_back(new Slice(x, t));
}

Slice* Track::get_slice(std::size_t index) {
    return slices_[index];
}

void Track::copy_slices(std::shared_ptr<Track> source) {
    clear_slices();
    std::size_t j = 0;
    for (auto i = source->slices_.size(); i-- > 0;) {
        Slice* slice = source->slices_[j++];
        add_slice(slice->x, slice->t);
    }
}

std::wstring Track::get_name() {
    return name_;
}

int32_t Track::get_local_y() {
    return local_y_;
}

void Track::set_local_y(int32_t local_y) {
    local_y_ = local_y;
}

int32_t Track::get_delta_x(uint32_t t, std::size_t lap_first_index, std::size_t lap_last_index,
        bool* is_previous_lap, bool* is_next_lap, bool* is_finish) {
    std::size_t size = slices_.size();

    if (size == 0) {
        *is_previous_lap = false;
        *is_next_lap = false;
        *is_finish = true;
        return 0;
    }

    std::size_t i = lap_first_index;
    Slice* slice = slices_[i];
    while (i <= lap_last_index && t >= slices_[i]->t) {
        slice = slices_[i];
        i++;
    }
    *is_previous_lap = t < slices_[lap_first_index]->t;
    // LOG(INFO) << "+++++ lap_last_index=" << lap_last_index << ", size=" << size;
    *is_next_lap = lap_last_index + 2 < size && t > slices_[lap_last_index + 1]->t;
    *is_finish = !(*is_next_lap) && t > slices_[size - 1]->t;
    if (*is_previous_lap)
        return -1;
    if (*is_next_lap)
        return Race::LAP_MAX_SIZE;
    if (*is_finish) {
        std::size_t x = (slice->x + 1) % Race::LAP_MAX_SIZE;
        return x == 0 ? Race::LAP_MAX_SIZE : x;
    }
    return slice->x % Race::LAP_MAX_SIZE;
}

void Track::set_finished(bool finished) {
    finished_ = finished;
    if (finished && race_)
        race_->finish_track(this);
}

bool Track::get_finished() {
    return finished_;
}

uint32_t Track::get_track_time() {
    std::size_t size = slices_.size();
    return size == 0 ? 0 : slices_[size - 1]->t;
}
