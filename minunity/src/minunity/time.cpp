// Copyright 2020 DmitryDzz

#include "time.hpp"

#include <chrono>

using minunity::Time;

Time::Time(const int max_fps) {
    start_ = std::chrono::steady_clock::now();
    frame_duration_ = std::chrono::duration<uint32_t, std::ratio<1, 1000>>(1000 / max_fps);
    millis_ = std::chrono::duration<uint32_t, std::ratio<1, 1000>>(0);
    delta_millis_ = std::chrono::duration<uint32_t, std::ratio<1, 1000>>(0);
    // update();
}

bool Time::update() {
    std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
    millis32 millis = std::chrono::duration_cast<millis32>(now - start_);
    millis32 delta_millis = millis - millis_;
    if (delta_millis >= frame_duration_) {
        millis_ = millis;
        delta_millis_ = delta_millis;
        return true;
    }
    return false;
}

uint32_t Time::get_time() {
    return millis_.count();
}

uint32_t Time::get_delta_time() {
    return delta_millis_.count();
}
