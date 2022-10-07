// Copyright 2021 DmitryDzz

#include <logger/easylogging++.h>

#include <iostream>
#include <fstream>
#include <regex>
#include <string>

#include <minunity/collider_groups.hpp>
#include <minunity/engine.hpp>
#include <minunity/game_exception.hpp>
#include <minunity/graph.hpp>

#include "app_manager.hpp"
#include "scenes/scene_level_1.hpp"

using keyzz::AppManager;
using keyzz::SceneLevel1;

using minunity::ColorInfo;
using minunity::ColliderGroups;
using minunity::Graph;

const char VERSION[] = "1.0.3";
const char INTRO[] = "keyzz© terminal game by @DmitryDzz";

void output_intro() {
    std::cout << INTRO << std::endl;
    std::cout << std::endl;
}

void output_help() {
    std::cout << "Usage" << std::endl;
    std::cout << "  keyzz <text file>" << std::endl;
    std::cout << "  keyzz --help|-h" << std::endl;
    std::cout << "  keyzz --version|-v" << std::endl;
}

void output_version() {
    // For CI / CD purposes, the output must contain only one line.
    std::cout << VERSION << std::endl;
}

int main(int argc, char** argv) {
    if (argc != 2) {
        output_intro();
        output_help();
        return AppManager::ERR_WRONG_PARAMETERS;
    }

    const std::string arg(argv[1]);
    std::smatch m;
    const std::regex re_help("--help|-h");
    const std::regex re_version("--version|-v");

    if (std::regex_match(arg, m, re_help)) {
        output_intro();
        output_help();
        return 0;
    }
    if (std::regex_match(arg, m, re_version)) {
        output_version();
        return 0;
    }

    output_intro();

    std::string filename(argv[1]);

    minunity::Engine *engine = minunity::Engine::get_instance();

    el::Helpers::setStorage(engine->get_log_storage());

    LOG(INFO) << INTRO;

    try {
        AppManager& app_manager = AppManager::get_instance();
        AppManager::Settings& settings = *app_manager.get_settings();
        app_manager.set_settings(settings.TAB_CODE, settings.RETURN_CODE, settings.USE_COLORS, true);

        engine->start();
        if (app_manager.get_settings()->USE_COLORS) {
            engine->init_color();

            if (engine->get_color_info().HAS_COLORS) {
                char primary = Graph::create_pair(1, -1, -1);
                if (engine->get_color_info().CAN_CHANGE_COLOR) {
                    int gray = 400;
                    Graph::change_color(COLOR_BLUE, gray, gray, gray);
                }
                char secondary = Graph::create_pair(2, COLOR_BLUE, -1);
                app_manager.set_color_pair_indexes(primary, secondary);
            }
        }

        // ColorInfo& info = engine->get_color_info();
        // LOG(INFO) << "+++++ [main.cpp] " <<
        //     " has_colors=" << info.HAS_COLORS <<
        //     " can_change_color=" << info.CAN_CHANGE_COLOR <<
        //     " COLORS=" << info.COLORS_COUNT <<
        //     " COLOR_PAIRS=" << info.COLOR_PAIRS_COUNT;

        ColliderGroups& collider_groups = engine->get_collider_groups();
        collider_groups.clear();
        const AppManager::ColliderGroup& collider_group = app_manager.collider_group;
        collider_groups.add_pair(collider_group.player, collider_group.border);
        collider_groups.add_pair(collider_group.enemy, collider_group.border);

        SceneLevel1 *sceneLevel1 = new SceneLevel1(filename, app_manager);
        engine->set_scene(sceneLevel1);

        engine->run();
        engine->stop();
        delete engine;

        return 0;
    } catch (minunity::GameException &e) {
        engine->stop();
        delete engine;

        std::cerr << "ERROR: " << e.what() << std::endl;
        return e.get_code();
    }


    // setlocale(LC_ALL, "");
    // initscr();

    // if (!has_colors()) {
    //     endwin();
    //     std::cout << "Terminal doesn't support colors" << std::endl;
    //     return -1;
    // }

    // start_color();
    // init_pair(1, COLOR_CYAN, COLOR_BLACK);

    // printw("Hello ncurses!\n");

    // attron(COLOR_PAIR(1));
    // //wchar_t playerChar = L'\u2588';
    // const char* idle = "█";
    // printw(idle);
    // // printw(idle + "\n");
    // // const char* jump0 = "▄";
    // // const char* jump1 = "▀";
    // // const char* jump2 = "█";
    // // printw(jump0 + "\n");
    // // printw(jump1 + "\n");
    // // printw(jump2 + "\n");
    // // printw("%c\n", (char)219);
    // printw("\n");
    // for (int i = 0; i < 5; i++) {
    //       printw("%c %lc\n", '0' + i, L'０' + i);
    // }
    // attroff(COLOR_PAIR(1));

    // refresh();
    // getch();
    // endwin();
}
