// Copyright 2021 DmitryDzz

#include "scene_level_1.hpp"

#include <logger/easylogging++.h>

#include <memory>

#include <minunity/engine.hpp>
#include <minunity/game_object.hpp>
#include <minunity/group_id.hpp>

// #include "../app_manager.hpp"
#include "../keyboard/shift_button.hpp"

using keyzz::Button;
using keyzz::SceneLevel1;
using keyzz::ShiftButton;
using keyzz::TextBlock;
using keyzz::Track;
using keyzz::MyRunner;
using keyzz::Runner;
using keyzz::StartDialog;
using keyzz::FinishDialog;
// using keyzz::AppManager;

using minunity::Engine;
using minunity::GameObject;

SceneLevel1::SceneLevel1(std::string filename, AppManager& app_manager) : Scene(),
    filename_(filename), app_manager_(app_manager) {
}

void SceneLevel1::awake() {
//    AppManager& app_manager = AppManager::get_instance();
    race_ = std::shared_ptr<Race>(new Race(this));
    race_->load(filename_);

    create_text_block();
    create_continue_button();
    create_keyboard();
    create_dialogs();
    create_lap_counter();
    create_error_counter();

    Scene::awake();

    start_dialog_->show();
}

void SceneLevel1::update() {
    Scene::update();

    bool time_to_open_finish_dialog = finish_dialog_show_millis_ &&
        Engine::get_instance()->get_time()->get_time() >= finish_dialog_show_millis_.value();

    if (time_to_open_finish_dialog || continue_button_pressed_) {
        text_block_->set_active(false);
        my_runner_->set_active(false);
        for (auto i = runners_.size(); i-- > 0;)
            runners_[i]->set_active(false);
        lap_counter_->set_active(false);
        continue_button_->set_active(false);

        finish_dialog_show_millis_ = std::nullopt;
        continue_button_pressed_ = false;

        finish_dialog_->show();
    }
}

void SceneLevel1::on_lap_start(std::wstring lap_text, std::optional<std::wstring> next_lap_text,
        int lap_index, int laps_count) {
    text_block_->start_lap(lap_text, next_lap_text, lap_index, laps_count);
    text_block_->set_active(true);

    my_runner_->set_active(true);
    my_runner_->start_lap(lap_index, laps_count);
    for (auto i = runners_.size(); i-- > 0;) {
        runners_[i]->set_active(true);
        runners_[i]->start_lap(lap_index, laps_count);
    }

    keyboard_->set_active(true);

    output_lap_counter(lap_index, laps_count);
}

void SceneLevel1::on_race_finished() {
    // Pause for a second, then hide TextBlock and runners, then show finish_dialog_.
    finish_dialog_show_millis_ = Engine::get_instance()->get_time()->get_time() + 1000;
}

void SceneLevel1::on_key_pressed(Button& sender, const int key) {
    if (!text_block_ || !my_runner_) return;

    // LOG(INFO) << "[SceneLevel1] key=" << key;
    if (!my_runner_->in_error_delay()) {
        if (text_block_->input(key)) {
            my_runner_->move_one_step();
        } else {
            if (app_manager_.get_settings()->USE_PENALTY) {
                my_runner_->start_error_delay();
            }
            errors_count_++;
            output_error_counter(errors_count_);
        }
    }

    if (text_block_->get_race_finished()) {
        my_runner_->finish();
        keyboard_->set_active(false);
        continue_button_->set_active(true);
    }

    if (text_block_->get_lap_finished()) {
        race_->next_lap();
    }
}

void SceneLevel1::on_continue_button_pressed(Button& sender) {
    continue_button_pressed_ = true;
}

void SceneLevel1::on_play() {
    race_->reload_tracks();
    create_runners();
    race_->start_race();

    errors_count_ = 0;
    output_error_counter(errors_count_);
}

void SceneLevel1::on_quit() {
    Engine::get_instance()->terminate();
}

void SceneLevel1::create_text_block() {
    text_block_ = std::dynamic_pointer_cast<TextBlock>(add_game_object(new TextBlock(3, 0)));
    text_block_->set_active(false);
}

void SceneLevel1::create_runners() {
    if (my_runner_ != nullptr)
        my_runner_->set_active(false);
    for (auto i = runners_.size(); i-- > 0;)
        runners_[i]->set_active(false);
    runners_.clear();

    std::shared_ptr<Track> my_track = race_->get_track(0);
    my_runner_ = std::dynamic_pointer_cast<MyRunner>(
            add_game_object(new MyRunner(2, 3 + my_track->get_local_y(), race_->get_track(0))));
    my_runner_->set_active(false);
    my_runner_->awake();
    my_runner_->start();

    for (int i = 0; i < race_->get_tracks_count() - 1; i++) {  // "- 1" is because track[0] goes to MyRunner.
        // Create as much runners as tracks left.
        std::shared_ptr<Track> track = race_->get_track(i + 1);
        std::shared_ptr<Runner> runner = std::dynamic_pointer_cast<Runner>(
            add_game_object(new Runner(2, 3 + track->get_local_y(), track)));
        runners_.push_back(runner);
        runner->set_active(false);
        runner->awake();
        runner->start();
    }
}

void SceneLevel1::create_keyboard() {
    keyboard_ = std::dynamic_pointer_cast<Keyboard>(add_game_object(new Keyboard(this, this)));
    keyboard_->set_active(false);
}

void SceneLevel1::create_dialogs() {
    start_dialog_ = std::dynamic_pointer_cast<StartDialog>(add_game_object(new StartDialog(this, this)));
    finish_dialog_ = std::dynamic_pointer_cast<FinishDialog>(add_game_object(new FinishDialog(this, this, race_)));
}

void SceneLevel1::create_lap_counter() {
    std::wstring text = L"Lap: 1/?";
    lap_counter_ = std::dynamic_pointer_cast<Label>(add_game_object(new Label(4, 22, 17, text)));
    lap_counter_->set_active(false);
}

void SceneLevel1::output_lap_counter(int lap_index, int laps_count) {
    std::wstring text = L"Lap: " + std::to_wstring(lap_index + 1) + L"/" + std::to_wstring(laps_count);
    lap_counter_->set_text(text);
    lap_counter_->set_active(true);
}

void SceneLevel1::create_error_counter() {
    std::wstring text = L"Errors: 0";
    error_counter_ = std::dynamic_pointer_cast<Label>(add_game_object(
                new Label(52, 22, 24, text, LabelAlignment::right)));
    error_counter_->set_active(false);
}

void SceneLevel1::output_error_counter(uint32_t errors_count) {
    std::wstring text = L"Errors: " + std::to_wstring(errors_count);
    error_counter_->set_text(text);
    error_counter_->set_active(true);
}

void SceneLevel1::create_continue_button() {
    continue_button_ = std::dynamic_pointer_cast<ContinueButton>(add_game_object(
                new ContinueButton(this, this)));
    continue_button_->set_active(false);
}
