// Copyright 2021 DmitryDzz

#include "app_manager.hpp"

#include <logger/easylogging++.h>

using keyzz::AppManager;

static AppManager* s_instance = nullptr;

AppManager& AppManager::get_instance() {
    if (!s_instance) {
        s_instance = new AppManager();
    }
    return *s_instance;
}

AppManager::AppManager() {
    settings_ = std::shared_ptr<Settings>(new Settings(0x27A7, 0x21B2, true, true));
}

void AppManager::set_settings(
        const int tab_code, const int return_code, const bool use_colors, const bool use_penalty) {
    settings_ = std::shared_ptr<Settings>(new Settings(tab_code, return_code, use_colors, use_penalty));
}

void AppManager::set_color_pair_indexes(const char primary, const char secondary) {
    color_pair_indexes_ = std::shared_ptr<ColorPairIndexes>(new ColorPairIndexes(primary, secondary));
}
