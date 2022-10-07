#ifndef TRACK_HPP_INCLUDED
#define TRACK_HPP_INCLUDED

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

namespace keyzz {

struct Slice {
public:
    int32_t x;
    uint32_t t;
    Slice();
    Slice(int32_t x, uint32_t t);
};

class Race;

class Track {
public:
    Track(Race* race, std::wstring name);
    int32_t get_id();

    std::size_t get_size();
    void clear_slices();
    void add_slice(int32_t x, uint32_t t);
    keyzz::Slice* get_slice(std::size_t index);
    void copy_slices(std::shared_ptr<Track> source);

    std::wstring get_name();
    int32_t get_delta_x(uint32_t t, std::size_t lap_first_index, std::size_t lap_last_index,
            bool* is_previous_lap, bool* is_next_lap, bool* is_finish);

    int32_t get_local_y();
    void set_local_y(int32_t local_y);

    bool get_finished();
    void set_finished(bool finished);

    uint32_t get_track_time();
private:
    Race* race_;
    std::wstring name_;
    int32_t local_y_ = 0;
    std::vector<keyzz::Slice*> slices_;
    bool finished_ = false;
};

}

#endif  // TRACK_HPP_INCLUDED
