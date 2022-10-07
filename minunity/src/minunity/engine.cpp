// Copyright 2020 DmitryDzz

#include "engine.hpp"

#include <ncurses.h>

#include <locale>
#include <thread>

#include "game_exception.hpp"
#include "input.hpp"

using minunity::ColorInfo;
using minunity::Engine;
using minunity::GameException;
using minunity::Time;
using minunity::Scene;
using minunity::ColliderGroups;

Engine *s_instance = nullptr;
const int Engine::DELAY_MICROSECONDS;

INITIALIZE_EASYLOGGINGPP

el::base::type::StoragePointer Engine::get_log_storage() {
    return el::Helpers::storage();
}

Engine *Engine::get_instance() {
    if (s_instance == nullptr) {
        el::Configurations conf;
        conf.setToDefault();
        conf.set(el::Level::Global, el::ConfigurationType::ToFile, "true");
// Use -DELPP_DEFAULT_LOG_FILE to set log filename.
//        conf.set(el::Level::Global, el::ConfigurationType::Filename, "log.txt");
        conf.set(el::Level::Global, el::ConfigurationType::ToStandardOutput, "false");
        el::Loggers::reconfigureLogger("default", conf);

        LOG(INFO) << "minunity© terminal game engine by @DmitryDzz";

        s_instance = new Engine();
    }
    return s_instance;
}

void winch_handler(int sig) {
    Engine *engine = Engine::get_instance();
    if (engine != nullptr)
        engine->redraw();
}

void Engine::redraw() {
    endwin();
    // Needs to be called after an endwin() so ncurses will initialize
    // itself with the new terminal dimensions.
    refresh();
    clear();

    // LOG(INFO) << "+++++++++ New size: " << COLS << "x" << LINES;

    if (scene_ != nullptr)
        scene_->redraw();  // Sets force_redraw_ flag in all RendererComponents.
}

void Engine::start() {
    setlocale(LC_ALL, "");
    initscr();
    color_info_ = std::shared_ptr<ColorInfo>(new ColorInfo(has_colors(), can_change_color(), COLORS, COLOR_PAIRS));
    curs_set(0);

    struct sigaction sa;
    memset(&sa, 0, sizeof(struct sigaction));
    sa.sa_handler = winch_handler;
    sigaction(SIGWINCH, &sa, NULL);

    Input::start();
    is_started_ = true;

    game_time->update();
}

void Engine::init_color() {
    if (has_colors()) {
        start_color();
        use_default_colors();
        color_info_ = std::shared_ptr<ColorInfo>(new ColorInfo(true, can_change_color(), COLORS, COLOR_PAIRS));
    }
}

ColorInfo& Engine::get_color_info() {
    return *color_info_;
}

void Engine::stop() {
    if (!is_started_) return;
    color_info_ = std::shared_ptr<ColorInfo>(new ColorInfo(has_colors(), can_change_color(), COLORS, COLOR_PAIRS));
    endwin();
    is_started_ = false;
}

void Engine::run() {
    if (!is_started_)
        throw new GameException(GameException::ENGINE_NOT_STARTED, "Engine not started");

    is_running_ = true;
    while (is_running_) {
        run_once_impl();
        std::this_thread::sleep_for(std::chrono::microseconds(Engine::DELAY_MICROSECONDS));
    }
}

void Engine::run_once() {
    if (!is_started_)
        throw new GameException(GameException::ENGINE_NOT_STARTED, "Engine not started");

    if (!is_running_) return;
    is_running_ = true;
    run_once_impl();
}

void Engine::run_once_impl() {
    if (!game_time->update())
        return;
    Input::update();
    if (scene_ != nullptr) {
        scene_->update();
        scene_->render();
    }
}

void Engine::terminate() {
    is_running_ = false;
}

Scene* Engine::get_scene() {
    return scene_;
}

void Engine::set_scene(Scene *scene) {
    if (scene_ != nullptr) {
        scene_->destroy();
        scene_ = nullptr;
    }

    scene_ = scene;

    if (scene_ != nullptr) {
        scene_->awake();
        scene_->start();
        scene_->update_colliders_list();
        collider_groups_.prepare(scene_->get_colliders());
    }
}

ColliderGroups& Engine::get_collider_groups() {
    return collider_groups_;
}

Time* Engine::get_time() {
    return game_time;
}

// void Engine::get_colliders(std::vector<std::shared_ptr<ColliderComponent>> &colliders) {
//     colliders.clear();
//     if (scene_ == nullptr) return;
//
//     auto i = std::begin(scene_->get_game_objects());
//     while (i != std::end(scene_->get_game_objects())) {
//         GameObject *go = (*i).get();
//
//         auto j = std::begin(go->get_colliders());
//         while (j != std::end(go->get_components())) {
//             Component *component = (*j).get();
//             if (component->get
//         }
//     }
// }
