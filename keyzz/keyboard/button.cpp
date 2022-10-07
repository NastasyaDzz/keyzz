// Copyright 2021 DmitryDzz

#include "button.hpp"
#include "shift_button.hpp"

#include <minunity/game_object.hpp>
#include <minunity/input.hpp>

using minunity::AnimationComponent;
using minunity::AnimatorComponent;
using minunity::Input;
using minunity::Layer;
using minunity::PositionComponent;
using minunity::Rect;
using minunity::RendererComponent;
using minunity::Sprite;

using keyzz::Button;

Button::Button(int x, int y, int w,
        std::wstring text, std::wstring pressed_text, std::wstring pressed_shift_text,
        std::optional<int> key_code, std::optional<int> shift_key_code,
        IButtonCallback *button_callback, std::shared_ptr<ShiftButton> shift_button,
        std::optional<char> color_pair_index) :
        x_(x),
        y_(y),
        w_(w < MIN_W ? MIN_W : w),
        text_(text),
        pressed_text_(pressed_text),
        pressed_shift_text_(pressed_shift_text),
        key_code_(key_code),
        shift_key_code_(shift_key_code),
        button_callback_(button_callback),
        shift_button_(shift_button),
        color_pair_index_(color_pair_index) {
    position_component_ = std::dynamic_pointer_cast<PositionComponent>(add_component(new PositionComponent()));
    position_component_->set_position(x, y);
    renderer_component_ = std::dynamic_pointer_cast<RendererComponent>(add_component(new RendererComponent()));
    if (color_pair_index_)
        renderer_component_->set_color_pair_index(color_pair_index_.value());

    idle_animation_ = std::dynamic_pointer_cast<AnimationComponent>(add_component(new AnimationComponent()));
    pressed_animation_ = std::dynamic_pointer_cast<AnimationComponent>(add_component(new AnimationComponent()));
    shift_pressed_animation_ = std::dynamic_pointer_cast<AnimationComponent>(add_component(new AnimationComponent()));

    animator_ = std::dynamic_pointer_cast<AnimatorComponent>(add_component(new AnimatorComponent()));
}

void Button::awake() {
    GameObject::awake();

    if (sprite_ != nullptr)
        delete sprite_;
    sprite_ = create_sprite();

    renderer_component_->init(position_component_.get(), sprite_, Layer::GUI_BACKGROUND);
    idle_animation_->init(sprite_, 0, false, { 0 });
    pressed_animation_->init(sprite_, 350, false, { 1, 0 });
    shift_pressed_animation_->init(sprite_, 350, false, { 2, 0 });

    animator_->add_animation(idle_animation_);
    animator_->add_animation(pressed_animation_);
    animator_->add_animation(shift_pressed_animation_);
    animator_->play(idle_animation_->get_id());
}

void Button::destroy() {
    if (!is_destroyed() && sprite_ != nullptr) {
        renderer_component_->clear();
        delete sprite_;
        sprite_ = nullptr;
    }
    GameObject::destroy();
}

std::wstring Button::build_frame(
        std::wstring left_top, std::wstring right_top,
        std::wstring left_bottom, std::wstring right_bottom,
        std::wstring horizontal, std::wstring vertical,
        std::wstring text) {
    std::wstring result(L"");
    result += left_top;
    for (int i = 0; i < w_ - 2; i++)
        result += horizontal;
    result += right_top;
    result += vertical;
    if (text.length() != static_cast<uint64_t>(w_ - 2)) {
        result += std::wstring(L"Err");
        for (int i = 3; i < w_ - 2; i++)
            result += std::wstring(L" ");
    } else {
        result += text;
    }
    result += vertical;
    result += left_bottom;
    for (int i = 0; i < w_ - 2; i++)
        result += horizontal;
    result += right_bottom;
    return result;
}

Sprite* Button::create_sprite() {
    Sprite *result = new Sprite(w_, 3, 3);
    std::wstring f1 = build_frame(L"┌", L"┐", L"└", L"┘", L"─", L"│", text_);
    std::wstring f2 = build_frame(L"┏", L"┓", L"┗", L"┛", L"━", L"┃", pressed_text_);
    std::wstring f3 = build_frame(L"┏", L"┓", L"┗", L"┛", L"━", L"┃", pressed_shift_text_);
    std::wstring res = f1 + f2 + f3;
    result->load(res.c_str());
    return result;
}

void Button::update() {
    GameObject::update();

    if (is_destroyed() || !get_active()) return;

    if (key_code_ && Input::is_key_pressed(key_code_.value())) {
        bool proceed = true;
        if (button_callback_)
            button_callback_->before_pressed(*this, key_code_.value(), &proceed);
        if (proceed) {
            animator_->play(pressed_animation_->get_id());
            if (button_callback_)
                button_callback_->after_pressed(*this, key_code_.value());
        }
    } else if (shift_key_code_ && Input::is_key_pressed(shift_key_code_.value())) {
        bool proceed = true;
        if (button_callback_)
            button_callback_->before_pressed(*this, shift_key_code_.value(), &proceed);
        if (proceed) {
            animator_->play(shift_pressed_animation_->get_id());
            if (button_callback_)
                button_callback_->after_pressed(*this, shift_key_code_.value());
            if (shift_button_)
                shift_button_->play_animation();
        }
    }
}
