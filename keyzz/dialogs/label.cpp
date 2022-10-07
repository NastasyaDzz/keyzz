// Copyright 2021 DmitryDzz

#include "label.hpp"

#include <logger/easylogging++.h>

#include <minunity/game_object.hpp>

using minunity::Point;
using minunity::Layer;
using minunity::PositionComponent;
using minunity::RendererComponent;
using minunity::Sprite;

using keyzz::Label;

Label::Label(int x, int y, int w, std::wstring text, LabelAlignment alignment)
        : x_(x), y_(y), w_(w < MIN_W ? MIN_W : w), text_(text), alignment_(alignment) {
    position_component_ = std::dynamic_pointer_cast<PositionComponent>(add_component(new PositionComponent()));
    position_component_->set_position(x, y);
    renderer_component_ = std::dynamic_pointer_cast<RendererComponent>(add_component(new RendererComponent()));
}

void Label::awake() {
    GameObject::awake();

    if (sprite_ != nullptr)
        delete sprite_;
    sprite_ = new Sprite(w_, 1, 1);
    set_text(text_);

    renderer_component_->init(position_component_.get(), sprite_, Layer::GUI_FOREGROUND);
}

void Label::destroy() {
    if (!is_destroyed() && sprite_ != nullptr) {
        renderer_component_->clear();
        delete sprite_;
        sprite_ = nullptr;
    }
    GameObject::destroy();
}

void Label::render_layer(minunity::Layer layer) {
    if (get_visible())
        GameObject::render_layer(layer);
}

std::wstring Label::get_text() {
    return text_;
}

void Label::set_text(std::wstring text) {
    std::wstring spaces(w_ - text.size(), L' ');
    text_ = alignment_ == LabelAlignment::right ? spaces + text : text + spaces;
    sprite_->load(text_.c_str());
}

void Label::set_position(Point& position) {
    position_component_->set_position(position);
}

void Label::set_position(int x, int y) {
    position_component_->set_position(x, y);
}

Point& Label::get_position() {
    return position_component_->get_position();
}
