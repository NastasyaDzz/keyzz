// Copyright 2021 DmitryDzz

#include "continue_button.hpp"

#include <logger/easylogging++.h>

#include <memory>

#include "../app_manager.hpp"

using keyzz::TextButton;
using keyzz::ContinueButton;

using minunity::GameObject;
using minunity::Scene;
using minunity::Sprite;

TextButton::TextButton(int x, int y, int w,
       std::wstring text, std::wstring pressed_text, std::wstring pressed_shift_text,
       std::optional<int> key_code, std::optional<int> shift_key_code,
       IButtonCallback *pressed_callback, std::shared_ptr<ShiftButton> shift_button,
       std::optional<char> color_pair_index) : Button(x, y, w, text, pressed_text, pressed_shift_text,
           key_code, shift_key_code, pressed_callback, shift_button, color_pair_index) {}

Sprite* TextButton::create_sprite() {
    Sprite *result = new Sprite(get_w(), 1, 3);
    std::wstring res = get_text() + get_pressed_text() + get_pressed_shift_text();
    result->load(res.c_str());
    return result;
}

ContinueButton::ContinueButton(Scene *scene, IContinueButtonPressedCallback *continue_callback)
    : GameObject(), scene_(scene), continue_callback_(continue_callback) {
    build();
}

void ContinueButton::set_active(bool active) {
    GameObject::set_active(active);
    if (button_)
        button_->set_visible(active);
}

void ContinueButton::before_pressed(Button& sender, const int key_code, bool* proceed) {
    *proceed = get_active();
}

void ContinueButton::after_pressed(Button& sender, const int key_code) {
    if (continue_callback_)
        continue_callback_->on_continue_button_pressed(sender);
}

void ContinueButton::build() {
    AppManager& app_manager = AppManager::get_instance();
    AppManager::ColorPairIndexes* colors = app_manager.get_color_pair_indexes();
    std::optional<char> color = std::nullopt;
    if (colors)
        color = colors->PRIMARY;

    std::wstring caption = L"Press to continue";
    button_ = std::dynamic_pointer_cast<TextButton>(scene_->add_game_object(new TextButton(28, 22,
                    static_cast<int>(caption.size()), caption, caption, caption,
                    static_cast<int>(' '), static_cast<int>(' '), this, nullptr, color)));
}
