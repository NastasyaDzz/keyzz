// Copyright 2021 DmitryDzz

#include "dialog.hpp"

#include <string>

#include <minunity/engine.hpp>

using minunity::Engine;
using minunity::Layer;
using minunity::PositionComponent;
using minunity::RendererComponent;
using minunity::Sprite;

using keyzz::Dialog;

Dialog::Dialog(int x, int y) : GameObject() {
    position_component_ = std::dynamic_pointer_cast<PositionComponent>(add_component(new PositionComponent()));
    position_component_->set_position(x, y);
    renderer_component_ = std::dynamic_pointer_cast<RendererComponent>(add_component(new RendererComponent()));
    set_active(false);
}

void Dialog::awake() {
    GameObject::awake();

    if (sprite_ != nullptr)
        delete sprite_;
    sprite_ = create_sprite();

    renderer_component_->init(position_component_.get(), sprite_, Layer::GUI_BACKGROUND);

    set_visible(false);
}

void Dialog::update() {
    GameObject::update();

    if (selected_btn_id_ && action_millis_) {
        auto delay_millis = Engine::get_instance()->get_time()->get_time() - action_millis_.value();
        if (delay_millis > 400) {
            auto btn_id = selected_btn_id_.value();
            selected_btn_id_ = std::nullopt;
            action_millis_ = std::nullopt;
            button_handler(btn_id);
        }
    }
}

void Dialog::set_visible(bool visible) {
    bool previous_visible = get_visible();
    set_active(visible);
    GameObject::set_visible(visible);
    if (previous_visible && !visible) {
        renderer_component_->clear();
    }
}

void Dialog::show() {
    set_visible(true);
}

void Dialog::hide() {
    set_visible(false);
}

void Dialog::before_pressed(Button& sender, const int key_code, bool* proceed) {
    *proceed = !selected_btn_id_;
}

void Dialog::after_pressed(Button& sender, const int key_code) {
    selected_btn_id_ = sender.get_id();
    action_millis_ = Engine::get_instance()->get_time()->get_time();
}
