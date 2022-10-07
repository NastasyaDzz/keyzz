#ifndef TIME_HPP_INCLUDED
#define TIME_HPP_INCLUDED

#include <chrono>

namespace minunity {

using millis32 = std::chrono::duration<uint32_t, std::ratio<1, 1000>>;

class Time {
public:
    Time(const int max_fps);
    bool update();
    uint32_t get_time();
    uint32_t get_delta_time();
private:
    std::chrono::steady_clock::time_point start_;
    millis32 frame_duration_;
    millis32 millis_;
    millis32 delta_millis_;
};

}

#endif // TIME_HPP_INCLUDED 
