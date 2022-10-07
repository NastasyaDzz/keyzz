// Copyright 2021 DmitryDzz

#include "race.hpp"

#include <logger/easylogging++.h>

#include <algorithm>
#include <cstring>
#include <cwchar>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <unordered_map>

#include <minunity/game_exception.hpp>

#include "../app_manager.hpp"

using keyzz::Track;
using keyzz::Race;

using minunity::GameException;

const std::size_t Race::LAP_MAX_SIZE;
const std::size_t Race::NEXT_LAP_SIZE;
const uint16_t Race::RECORDS_FILE_VERSION;

bool s_first_load = true;

Race::Race(IRaceCallback* race_callback) : race_callback_(race_callback), race_text_(L"") {
}

void Race::load_text(std::string text_filename, std::wstring* race_text) {
    std::wstring& rt = *race_text;
    if (!std::filesystem::exists(text_filename))
        throw GameException(AppManager::ERR_FILE_NOT_FOUND, "File not found (" + text_filename + ")");
    std::wifstream fin;
    fin.open(text_filename, std::ios::in);
    if (fin.fail())
        throw GameException(AppManager::ERR_CANT_OPEN_FILE, "Can't open file (" + text_filename + ")");
    try {
        std::wstringstream buffer;
        buffer << fin.rdbuf();
        rt = buffer.str();
    } catch(...) {
        fin.close();
        throw GameException(AppManager::ERR_CANT_READ_FILE, "Can't read file (" + text_filename + ")");
    }
    fin.close();

    if (rt.size() == 0)
        throw GameException(AppManager::ERR_FILE_IS_EMPTY, "File is empty (" + text_filename + ")");

    AppManager::Settings* settings = AppManager::get_instance().get_settings();
    rt = std::regex_replace(rt, std::wregex(L"\\n+"), std::wstring(1, settings->RETURN_CODE));
    rt = std::regex_replace(rt, std::wregex(L"\\t+"), std::wstring(1, settings->TAB_CODE));
    rt = std::regex_replace(rt, std::wregex(L"\\s+"), L" ");
}

std::string get_previous_records_filename(std::string text_filename) {
    return text_filename + ".rec";
}

void Race::load_records(std::string records_filename, std::vector<std::shared_ptr<Track>>* record_tracks) {
    // <char[16] header>  // must be equal to "keyzz-records   "
    // <uint16_t version>  // must be equal to 1
    // <std::size_t race_text_hash>
    // <uint32_t tracks_count>
    //   <uint32_t track0_slices_count>
    //     <int32_t x0> <uint32_t t0>
    //     <...>
    //   <uint32_t track1_slices_count>
    //     <int32_t x0> <uint32_t t0>
    //     <...>
    //   <...>

    record_tracks->clear();
    std::wstring track_names[] { L"(a)", L"(b)", L"(c)", L"(d)", L"(e)" };
    std::ifstream fin;
    fin.open(records_filename, std::ios::in | std::ios::binary);
    if (!fin.fail()) {
        try {
            int header_size = RECORDS_FILE_HEADER.size();
            std::string header(header_size, ' ');
            fin.read(reinterpret_cast<char *>(&header[0]), header_size);
            if (header != RECORDS_FILE_HEADER)
                throw std::runtime_error("Bad records file");

            uint16_t version;
            fin.read(reinterpret_cast<char *>(&version), sizeof(version));
            if (version != Race::RECORDS_FILE_VERSION)
                throw std::runtime_error("Wrong records file version");

            std::size_t hash;
            fin.read(reinterpret_cast<char *>(&hash), sizeof(hash));
            if (hash != race_text_hash_)
                throw std::runtime_error("Wrong hash");

            uint32_t tracks_count;
            fin.read(reinterpret_cast<char *>(&tracks_count), sizeof(tracks_count));
            if (tracks_count > Race::TRACKS_MAX_COUNT - 1) tracks_count = Race::TRACKS_MAX_COUNT - 1;
            for (int i = 0; i < static_cast<int>(tracks_count); i++) {
                auto track = std::shared_ptr<Track>(new Track(this, track_names[i]));
                record_tracks->push_back(track);
                uint32_t slices_count;
                fin.read(reinterpret_cast<char *>(&slices_count), sizeof(slices_count));
                for (int j = 0; j < static_cast<int>(slices_count); j++) {
                    int32_t x;
                    uint32_t t;
                    fin.read(reinterpret_cast<char *>(&x), sizeof(x));
                    fin.read(reinterpret_cast<char *>(&t), sizeof(t));
                    track->add_slice(x, t);
                }
            }
            fin.close();
        } catch(const std::exception &e) {
            LOG(ERROR) << e.what();
            fin.close();
        }
    }
}

void Race::save_records(std::string records_filename) {
    std::ofstream fout;
    fout.open(records_filename, std::ios::out | std::ios::trunc | std::ios::binary);
    try {
        fout.write(reinterpret_cast<const char *>(&RECORDS_FILE_HEADER[0]), sizeof(char) * RECORDS_FILE_HEADER.size());

        fout.write(reinterpret_cast<const char *>(&RECORDS_FILE_VERSION), sizeof(RECORDS_FILE_VERSION));

        fout.write(reinterpret_cast<const char *>(&race_text_hash_), sizeof(race_text_hash_));

        uint32_t tracks_count = tracks_.size();
        fout.write(reinterpret_cast<const char *>(&tracks_count), sizeof(tracks_count));
        for (int i = 0; i < static_cast<int>(tracks_count); i++) {
            Track* track = tracks_[i].get();
            uint32_t slices_count = track->get_size();
            fout.write(reinterpret_cast<const char *>(&slices_count), sizeof(slices_count));
            // LOG(INFO) << "\nTrack: " << track->get_name() << ", slices: " << track->get_size();
            for (int j = 0; j < static_cast<int>(track->get_size()); j++) {
                Slice* slice = track->get_slice(j);
                fout.write(reinterpret_cast<const char *>(&(slice->x)), sizeof(slice->x));
                fout.write(reinterpret_cast<const char *>(&(slice->t)), sizeof(slice->t));
                // LOG(INFO) << "Slice: " << j << ", x: " << slice->x << ", t: " << slice->t;
            }
        }
        fout.close();
    } catch(const std::exception &e) {
        LOG(ERROR) << e.what();
        fout.close();
    }
}

void Race::reload_tracks() {
    if (records_filename_)
        load_tracks(records_filename_.value(), &tracks_);
}

void Race::load_tracks(std::string records_filename, std::vector<std::shared_ptr<Track>>* tracks) {
    std::vector<std::shared_ptr<Track>> &all_tracks = *tracks;
    all_tracks.clear();

    auto track_u_ = std::shared_ptr<Track>(new Track(this, L"(U)"));
    all_tracks.push_back(track_u_);

    std::vector<std::shared_ptr<Track>> file_tracks;
    load_records(records_filename, &file_tracks);
    // Append all_tracks with file_tracks:
    for (int i = 0; i < static_cast<int>(file_tracks.size()); i++) {
        auto track = std::shared_ptr<Track>(new Track(this, file_tracks[i]->get_name()));
        track->copy_slices(file_tracks[i]);
        all_tracks.push_back(track);
    }

    set_local_y_on_tracks(all_tracks);
}

void Race::load(std::string text_filename) {
    load_text(text_filename, &race_text_);
    race_text_hash_ = std::hash<std::wstring>{}(race_text_);
    records_filename_ = get_previous_records_filename(text_filename);
    laps_count_ = race_text_.size() / LAP_MAX_SIZE + (race_text_.size() % LAP_MAX_SIZE == 0 ? 0 : 1);
    lap_index_ = 0;
}

void Race::save() {
    if (records_filename_)
        save_records(records_filename_.value());
}

int Race::get_tracks_count() {
    return tracks_.size();
}

std::shared_ptr<Track> Race::get_track(int index) {
    return tracks_[index];
}

void Race::start_race() {
    for (auto i = tracks_.size(); i-- > 0;) {
        Track* track = tracks_[i].get();
        track->set_finished(false);
    }

    // Start lap:
    lap_index_ = 0;
    if (race_callback_)
        race_callback_->on_lap_start(
                get_lap_text(race_text_, lap_index_),
                get_next_lap_text(race_text_, lap_index_),
                lap_index_,
                laps_count_);
}

void Race::next_lap() {
    if (lap_index_ >= laps_count_ - 1) return;
    // Start next lap:
    lap_index_++;
    if (race_callback_)
        race_callback_->on_lap_start(
                get_lap_text(race_text_, lap_index_),
                get_next_lap_text(race_text_, lap_index_),
                lap_index_,
                laps_count_);
}

std::wstring Race::get_lap_text(std::wstring race_text, std::size_t lap_index) {
    std::size_t index = lap_index * Race::LAP_MAX_SIZE;
    std::size_t count = std::min(Race::LAP_MAX_SIZE, race_text.size() - index);
    return race_text.substr(index, count);
}

std::optional<std::wstring> Race::get_next_lap_text(std::wstring race_text, std::size_t lap_index) {
    std::size_t index = (lap_index + 1) * Race::LAP_MAX_SIZE;
    if (race_text.size() <= index) {
        return std::nullopt;
    }
    std::size_t count = std::min(Race::NEXT_LAP_SIZE, race_text.size() - index);
    std::wstring result = index < race_text.size() ? race_text.substr(index, count) : L"";
    result += std::wstring(Race::NEXT_LAP_SIZE - result.size(), L' ');
    return result;
}

void Race::finish_track(Track *track) {
    std::size_t finished_tracks = 0;
    for (auto i = tracks_.size(); i-- > 0;)
        if (tracks_[i]->get_finished())
            finished_tracks++;
    if (finished_tracks == tracks_.size() && race_callback_)
        race_callback_->on_race_finished();
}

void Race::set_local_y_on_tracks(std::vector<std::shared_ptr<Track>> tracks) {
    switch (tracks.size()) {
        case 1:
            tracks[0]->set_local_y(2);
            break;
        case 2:
            tracks[0]->set_local_y(1);
            tracks[1]->set_local_y(3);
            break;
        case 3:
            tracks[0]->set_local_y(0);
            tracks[1]->set_local_y(2);
            tracks[2]->set_local_y(4);
            break;
        default:
            for (auto i = tracks_.size(); i-- > 0;)
                tracks[i]->set_local_y(i);
            break;
    }
}

bool cmp_tracks(const std::shared_ptr<Track> a, const std::shared_ptr<Track> b) {
    uint32_t t1 = a->get_track_time();
    uint32_t t2 = b->get_track_time();
    if (t1 < t2) return true;
    if (t1 > t2) return false;
    return a->get_name().compare(b->get_name()) < 0;
}

void Race::sort_by_track_time() {
    std::sort(tracks_.begin(), tracks_.end(), cmp_tracks);
}
