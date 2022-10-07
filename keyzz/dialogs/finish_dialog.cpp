// Copyright 2021 DmitryDzz

#include "finish_dialog.hpp"

#include <logger/easylogging++.h>

#include <algorithm>
#include <string>

#include <minunity/engine.hpp>
#include <minunity/graph.hpp>

#include "../tracks/race.hpp"

using minunity::Engine;
using minunity::Graph;
using minunity::Point;
using minunity::PositionComponent;
using minunity::RendererComponent;
using minunity::Sprite;

using keyzz::FinishDialog;
using keyzz::Label;
using keyzz::Race;

const int Race::TRACKS_MAX_COUNT;

const int RECORDS_FIRST_ROW = 3;

FinishDialog::FinishDialog(minunity::Scene *scene, IFinishDialogCallback *dialog_callback, std::shared_ptr<Race> race)
    : Dialog(22, 0), dialog_callback_(dialog_callback), race_(race) {
    std::wstring caption = L" (P)lay ";
    play_btn_ = std::dynamic_pointer_cast<Button>(scene->add_game_object(
                new Button(48, 2, 10, caption, caption, caption, 'p', 'P', this)));
    play_btn_->set_active(false);

    caption = L" (Q)uit ";
    quit_btn_ = std::dynamic_pointer_cast<Button>(scene->add_game_object(
                new Button(48, 5, 10, caption, caption, caption, 'q', 'Q', this)));
    quit_btn_->set_active(false);

    title_label_ = std::dynamic_pointer_cast<Label>(scene->add_game_object(new Label(23, 1, 36, L"")));
    title_label_->set_active(false);

    position_label_ = std::dynamic_pointer_cast<Label>(scene->add_game_object(
                new Label(23, RECORDS_FIRST_ROW, 2, L" >")));
    position_label_->set_active(false);

    for (int i = 0; i < Race::TRACKS_MAX_COUNT; i++) {
        std::shared_ptr<Label> record = std::dynamic_pointer_cast<Label>(scene->add_game_object(
                    new Label(22 + 6, RECORDS_FIRST_ROW + i, 18, L"..................")));  // L"(U).....12.012"
        record->set_active(false);
        record_labels_.push_back(record);
    }
}

void FinishDialog::awake() {
    Dialog::awake();
}

Sprite* FinishDialog::create_sprite() {
    Sprite *result = new Sprite(38, 9, 1);
    std::wstring s = L"";
    //               1         2         3
    //     01234567890123456789012345678901234567
    s += L"╔════════════════════════════════════╗";  // 0
    s += L"║                                    ║";  // 1
    s += L"║                         ┌────────┐ ║";  // 2
    s += L"║  1. (U).........123.45  │ (P)lay │ ║";  // 3
    s += L"║  2. ..................  └────────┘ ║";  // 4
    s += L"║  3. ..................  ┌────────┐ ║";  // 5
    s += L"║  4. ..................  │ (Q)uit │ ║";  // 6
    s += L"║  5. ..................  └────────┘ ║";  // 7
    s += L"╚════════════════════════════════════╝";  // 8
    result->load(s.c_str());
    return result;
}

void FinishDialog::set_visible(bool visible) {
    Dialog::set_visible(visible);
    play_btn_->set_active(visible);
    quit_btn_->set_active(visible);
    title_label_->set_visible(visible);
    title_label_->set_active(visible);
    position_label_->set_visible(visible);
    position_label_->set_active(visible);
    for (int i = 0; i < Race::TRACKS_MAX_COUNT; i++)
        record_labels_[i]->set_active(visible);
    if (visible)
        draw_race_results();
}

void FinishDialog::button_handler(int64_t button_id) {
    set_visible(false);
    if (dialog_callback_) {
        if (button_id == play_btn_->get_id())
            dialog_callback_->on_play();
        else if (button_id == quit_btn_->get_id())
            dialog_callback_->on_quit();
    }
}

struct TrackResult {
    std::wstring name;
    uint32_t time;
};

bool cmp_track_results(const TrackResult &a, const TrackResult &b) {
    uint32_t t1 = a.time == 0 ? 0xFFFFFFFF : a.time;
    uint32_t t2 = b.time == 0 ? 0xFFFFFFFF : b.time;
    if (t1 < t2) return true;
    if (t1 > t2) return false;
    return a.name.compare(b.name) < 0;
}

static std::wstring millis_to_wstring(uint32_t millis) {
    uint32_t m = millis / 60000;
    uint32_t delta_s = millis - m * 60000;
    uint32_t s = delta_s / 1000;
    uint32_t ms = delta_s - s * 1000;

    std::wstring ms_text = std::to_wstring(ms);
    ms_text = std::wstring(3 - ms_text.size(), L'0') + ms_text;

    std::wstring s_text = std::to_wstring(s);
    if (m > 0) s_text = std::wstring(2 - s_text.size(), L'0') + s_text;
    s_text += L".";

    std::wstring m_text = m > 0 ? std::to_wstring(m) + L":" : L"";

    return m_text + s_text + ms_text;
}

void FinishDialog::draw_race_results() {
    race_->sort_by_track_time();
    race_->save();

    int player_position = -1;
    for (int i = 0; i < race_->get_tracks_count(); i++) {
        std::wstring upper_name = race_->get_track(i)->get_name();
        std::transform(upper_name.begin(), upper_name.end(), upper_name.begin(), std::towupper);

        if (upper_name == L"(U)") {
            player_position = i;
            upper_name = L"(yoU)";

            Point p = position_label_->get_position();
            position_label_->set_position(p.x, RECORDS_FIRST_ROW + i);
            position_label_->set_visible(true);
            position_label_->set_active(true);
        }

        uint32_t time_value = race_->get_track(i)->get_track_time();
        std::wstring time_text = time_value == 0 ? L"" : millis_to_wstring(time_value);
        std::wstring dots = L"";
        dots.resize(18 - upper_name.size() - time_text.size(), L'.');
        record_labels_[i]->set_text(upper_name + dots + time_text);
    }

    if (player_position == 0)
        title_label_->set_text(L"              You win!");
    else if (player_position == 1)
        title_label_->set_text(L"     Second is the first loser!");
    else if (player_position == 2)
        title_label_->set_text(L"     Third is the second loser!");
    else if (player_position == Race::TRACKS_MAX_COUNT - 1)
        title_label_->set_text(L"             Epic loser!");
    else
        title_label_->set_text(L"               Loser!");
}
