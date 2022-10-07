#ifndef TRACKS_HPP_INCLUDED
#define TRACKS_HPP_INCLUDED

#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "track.hpp"

namespace keyzz {

class IRaceCallback {
public:
    virtual void on_lap_start(std::wstring lap_text, std::optional<std::wstring> next_lap_text, int lap_index, int laps_count) = 0;
    virtual void on_race_finished() = 0;
};

class Race {
public:
    static const std::size_t LAP_MAX_SIZE = 66;
    static const std::size_t NEXT_LAP_SIZE = 8;  // "Next lap" text size
    static const int TRACKS_MAX_COUNT = 5;
    inline static const std::string RECORDS_FILE_HEADER = "keyzz-records   ";
    static const uint16_t RECORDS_FILE_VERSION = 1;

    Race(IRaceCallback* race_callback);

    void load(std::string text_filename);
    void save();
    void reload_tracks();

    void start_race();
    void next_lap();
    void finish_track(Track *track);

    int get_tracks_count();
    std::shared_ptr<Track> get_track(int index);

    void sort_by_track_time();
private:
    std::optional<std::string> records_filename_ = std::nullopt;
    IRaceCallback* race_callback_;
    std::wstring race_text_;
    std::size_t race_text_hash_ = 0;
    std::vector<std::shared_ptr<Track>> tracks_;

    void load_text(std::string text_filename, std::wstring* race_text);
    void load_tracks(std::string records_filename, std::vector<std::shared_ptr<Track>>* tracks);
    void load_records(std::string records_filename, std::vector<std::shared_ptr<Track>>* record_tracks);
    void save_records(std::string records_filename);

    std::size_t laps_count_ = 0;
    std::size_t lap_index_ = 0;
    static std::wstring get_lap_text(std::wstring race_text, std::size_t lap_index);
    static std::optional<std::wstring> get_next_lap_text(std::wstring race_text, std::size_t lap_index);
    void set_local_y_on_tracks(std::vector<std::shared_ptr<Track>> tracks);
};

}

#endif  // TRACKS_HPP_INCLUDED
