// Copyright 2021 DmitryDzz

#include "keyboard.hpp"

#include <logger/easylogging++.h>

#include <memory>

#include "../keyboard/shift_button.hpp"

#include "../app_manager.hpp"

using keyzz::Button;
using keyzz::ShiftButton;
using keyzz::Keyboard;

using minunity::GameObject;
using minunity::Scene;

Keyboard::Keyboard(Scene *scene, IKeyboardCallback *keyboard_callback)
    : GameObject(), scene_(scene), keyboard_callback_(keyboard_callback) {
    build();
}

void Keyboard::before_pressed(Button& sender, const int key_code, bool* proceed) {
    *proceed = get_active();
}

void Keyboard::after_pressed(Button& sender, const int key_code) {
    if (keyboard_callback_)
        keyboard_callback_->on_key_pressed(sender, key_code);
}

void Keyboard::build() {
    AppManager& app_manager = AppManager::get_instance();
    AppManager::ColorPairIndexes* colors = app_manager.get_color_pair_indexes();
    std::optional<char> color4L = std::nullopt;
    std::optional<char> color3L = std::nullopt;
    std::optional<char> color2L = std::nullopt;
    std::optional<char> color1L = std::nullopt;
    std::optional<char> color1R = std::nullopt;
    std::optional<char> color2R = std::nullopt;
    std::optional<char> color3R = std::nullopt;
    std::optional<char> color4R = std::nullopt;
    if (colors != nullptr) {
        color4L = colors->SECONDARY;
        color3L = colors->PRIMARY;
        color2L = colors->SECONDARY;
        color1L = colors->PRIMARY;
        color1R = colors->SECONDARY;
        color2R = colors->PRIMARY;
        color3R = colors->SECONDARY;
        color4R = colors->PRIMARY;
    }

    std::shared_ptr<ShiftButton> shiftL = std::dynamic_pointer_cast<ShiftButton>(
            scene_->add_game_object(new ShiftButton(3, 18, 11, L" Shift L ", color4L)));
    std::shared_ptr<ShiftButton> shiftR = std::dynamic_pointer_cast<ShiftButton>(
            scene_->add_game_object(new ShiftButton(64, 18, 13, L"  Shift R  ", color4R)));
    int x = 3;
    int y = 9;
    scene_->add_game_object(new Button(x, y, 5, L"`╱~", L" ` ", L" ~ ", '`', '~', this, shiftR, color4L));
    scene_->add_game_object(new Button(x+=5, y, 5, L"1╱!", L" 1 ", L" ! ", '1', '!', this, shiftR, color4L));
    scene_->add_game_object(new Button(x+=5, y, 5, L"2╱@", L" 2 ", L" @ ", '2', '@', this, shiftR, color3L));
    scene_->add_game_object(new Button(x+=5, y, 5, L"3╱#", L" 3 ", L" # ", '3', '#', this, shiftR, color2L));
    scene_->add_game_object(new Button(x+=5, y, 5, L"4╱$", L" 4 ", L" $ ", '4', '$', this, shiftR, color1L));
    scene_->add_game_object(new Button(x+=5, y, 5, L"5╱%", L" 5 ", L" % ", '5', '%', this, shiftR, color1L));
    scene_->add_game_object(new Button(x+=5, y, 5, L"6╱^", L" 6 ", L" ^ ", '6', '^', this, shiftR, color1R));
    scene_->add_game_object(new Button(x+=5, y, 5, L"7╱&", L" 7 ", L" & ", '7', '&', this, shiftL, color1R));
    scene_->add_game_object(new Button(x+=5, y, 5, L"8╱*", L" 8 ", L" * ", '8', '*', this, shiftL, color2R));
    scene_->add_game_object(new Button(x+=5, y, 5, L"9╱(", L" 9 ", L" ( ", '9', '(', this, shiftL, color3R));
    scene_->add_game_object(new Button(x+=5, y, 5, L"0╱)", L" 0 ", L" ) ", '0', ')', this, shiftL, color4R));
    scene_->add_game_object(new Button(x+=5, y, 5, L"-╱_", L" - ", L" _ ", '-', '_', this, shiftL, color4R));
    scene_->add_game_object(new Button(x+=5, y, 5, L"=╱+", L" = ", L" + ", '=', '+', this, shiftL, color4R));
    scene_->add_game_object(new Button(x+=5, y, 9, L"  Back ", L"  Back ", L"  Back ", 0x107, 0x107, this,
                shiftL, color4R));
    x = 3;
    y += 3;
    scene_->add_game_object(new Button(x, y, 7, L" Tab ", L" Tab ", L" TAB ", 0x09, 0x161, this, shiftR, color4L));
    scene_->add_game_object(new Button(x+=7, y, 5, L" Q ", L" q ", L" Q ", 'q', 'Q', this, shiftR, color4L));
    scene_->add_game_object(new Button(x+=5, y, 5, L" W ", L" w ", L" W ", 'w', 'W', this, shiftR, color3L));
    scene_->add_game_object(new Button(x+=5, y, 5, L" E ", L" e ", L" E ", 'e', 'E', this, shiftR, color2L));
    scene_->add_game_object(new Button(x+=5, y, 5, L" R ", L" r ", L" R ", 'r', 'R', this, shiftR, color1L));
    scene_->add_game_object(new Button(x+=5, y, 5, L" T ", L" t ", L" T ", 't', 'T', this, shiftR, color1L));
    scene_->add_game_object(new Button(x+=5, y, 5, L" Y ", L" y ", L" Y ", 'y', 'Y', this, shiftL, color1R));
    scene_->add_game_object(new Button(x+=5, y, 5, L" U ", L" u ", L" U ", 'u', 'U', this, shiftL, color1R));
    scene_->add_game_object(new Button(x+=5, y, 5, L" I ", L" i ", L" I ", 'i', 'I', this, shiftL, color2R));
    scene_->add_game_object(new Button(x+=5, y, 5, L" O ", L" o ", L" O ", 'o', 'O', this, shiftL, color3R));
    scene_->add_game_object(new Button(x+=5, y, 5, L" P ", L" p ", L" P ", 'p', 'P', this, shiftL, color4R));
    scene_->add_game_object(new Button(x+=5, y, 5, L"[╱{", L" [ ", L" { ", '[', '{', this, shiftL, color4R));
    scene_->add_game_object(new Button(x+=5, y, 5, L"]╱}", L" ] ", L" } ", ']', '}', this, shiftL, color4R));
    scene_->add_game_object(new Button(x+=5, y, 7, L" \\╱| ", L"  \\  ", L"  |  ", '\\', '|', this, shiftL, color4R));
    x = 3;
    y += 3;
    scene_->add_game_object(new Button(x, y, 8, L" Caps ", L" Caps ", L" Caps ", std::nullopt, std::nullopt, nullptr,
                nullptr, color4L));
    scene_->add_game_object(new Button(x+=8, y, 5, L" A ", L" a ", L" A ", 'a', 'A', this, shiftR, color4L));
    scene_->add_game_object(new Button(x+=5, y, 5, L" S ", L" s ", L" S ", 's', 'S', this, shiftR, color3L));
    scene_->add_game_object(new Button(x+=5, y, 5, L" D ", L" d ", L" D ", 'd', 'D', this, shiftR, color2L));
    scene_->add_game_object(new Button(x+=5, y, 5, L" F ", L" f ", L" F ", 'f', 'F', this, shiftR, color1L));
    scene_->add_game_object(new Button(x+=5, y, 5, L" G ", L" g ", L" G ", 'g', 'G', this, shiftR, color1L));
    scene_->add_game_object(new Button(x+=5, y, 5, L" H ", L" h ", L" H ", 'h', 'H', this, shiftL, color1R));
    scene_->add_game_object(new Button(x+=5, y, 5, L" J ", L" j ", L" J ", 'j', 'J', this, shiftL, color1R));
    scene_->add_game_object(new Button(x+=5, y, 5, L" K ", L" k ", L" K ", 'k', 'K', this, shiftL, color2R));
    scene_->add_game_object(new Button(x+=5, y, 5, L" L ", L" l ", L" L ", 'l', 'L', this, shiftL, color3R));
    scene_->add_game_object(new Button(x+=5, y, 5, L";╱:", L" ; ", L" : ", ';', ':', this, shiftL, color4R));
    scene_->add_game_object(new Button(x+=5, y, 5, L"'╱\"", L" ' ", L" \" ", '\'', '"', this, shiftL, color4R));
    scene_->add_game_object(new Button(x+=5, y, 11, L"  Enter  ", L"  Enter  ", L"  Enter  ", 0x0A, 0x0A, this,
                shiftL, color4R));
    x = 14;
    y += 3;
    scene_->add_game_object(new Button(x, y, 5, L" Z ", L" z ", L" Z ", 'z', 'Z', this, shiftR, color4L));
    scene_->add_game_object(new Button(x+=5, y, 5, L" X ", L" x ", L" X ", 'x', 'X', this, shiftR, color3L));
    scene_->add_game_object(new Button(x+=5, y, 5, L" C ", L" c ", L" C ", 'c', 'C', this, shiftR, color2L));
    scene_->add_game_object(new Button(x+=5, y, 5, L" V ", L" v ", L" V ", 'v', 'V', this, shiftR, color1L));
    scene_->add_game_object(new Button(x+=5, y, 5, L" B ", L" b ", L" B ", 'b', 'B', this, shiftR, color1L));
    scene_->add_game_object(new Button(x+=5, y, 5, L" N ", L" n ", L" N ", 'n', 'N', this, shiftL, color1R));
    scene_->add_game_object(new Button(x+=5, y, 5, L" M ", L" m ", L" M ", 'm', 'M', this, shiftL, color1R));
    scene_->add_game_object(new Button(x+=5, y, 5, L",╱<", L" , ", L" < ", ',', '<', this, shiftL, color2R));
    scene_->add_game_object(new Button(x+=5, y, 5, L".╱>", L" . ", L" > ", '.', '>', this, shiftL, color3R));
    scene_->add_game_object(new Button(x+=5, y, 5, L"/╱?", L" / ", L" ? ", '/', '?', this, shiftL, color4R));
    x = 21;
    y += 3;
    scene_->add_game_object(new Button(x, y, 31, L"                             ", L"                             ",
                L"                             ", ' ', ' ', this));
}
