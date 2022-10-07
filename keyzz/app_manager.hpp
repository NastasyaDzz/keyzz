#ifndef KEYZZ_APP_MANAGER_HPP_INCLUDED
#define KEYZZ_APP_MANAGER_HPP_INCLUDED

#include <memory>

#include <minunity/group_id.hpp>

// #include "player.hpp"

namespace keyzz {

class AppManager {
public:
    static const int ERR_WRONG_PARAMETERS = -1;
    static const int ERR_FILE_NOT_FOUND = -2;
    static const int ERR_CANT_OPEN_FILE = -3;
    static const int ERR_CANT_READ_FILE = -4;
    static const int ERR_FILE_IS_EMPTY = -5;
    static const int ERR_ENGINE_NOT_STARTED = -6;
    static const int ERR_WRONG_SPRITE_SIZE = -7;

    static AppManager& get_instance();
    
    AppManager();

    class ColliderGroup {
    public:
        const minunity::group_id player = 0;
        const minunity::group_id border = 1;
        const minunity::group_id enemy = 2;
    };

    const ColliderGroup collider_group;

    class Settings {
    public:
        Settings(const int tab_code, const int return_code, const bool use_colors, const bool use_penalty) :
            TAB_CODE(tab_code), RETURN_CODE(return_code), USE_COLORS(use_colors), USE_PENALTY(use_penalty) {}

        const int TAB_CODE;
        const int RETURN_CODE;
        const bool USE_COLORS;
        const bool USE_PENALTY;
    };

    class ColorPairIndexes {
    public:
        ColorPairIndexes(const char primary, const char secondary) :
            PRIMARY(primary), SECONDARY(secondary) {}

        const char PRIMARY;
        const char SECONDARY;
    };

    Settings* get_settings() { return settings_.get(); }
    void set_settings(const int tab_code, const int return_code, const bool use_colors, const bool use_penalty);

    ColorPairIndexes* get_color_pair_indexes() { return color_pair_indexes_.get(); }
    void set_color_pair_indexes(const char primary, const char secondary);
private:
    std::shared_ptr<Settings> settings_ = nullptr;
    std::shared_ptr<ColorPairIndexes> color_pair_indexes_ = nullptr;
};

}

#endif // KEYZZ_APP_MANAGER_HPP_INCLUDED
