// Copyright 2021 DmitryDzz

#include "base_runner.hpp"

#include <minunity/game_object.hpp>

using minunity::GameObject;
using minunity::Layer;
using minunity::PositionComponent;
using minunity::RendererComponent;
using minunity::Sprite;

using keyzz::BaseRunner;

BaseRunner::BaseRunner(int x, int y, std::shared_ptr<Track> track_record)
    : GameObject(), start_x_(x), y_(y), lap_index_(0), laps_count_(1), track_(track_record) {
    position_component_ = std::dynamic_pointer_cast<PositionComponent>(add_component(new PositionComponent()));
    position_component_->set_position(x, y);
    renderer_component_ = std::dynamic_pointer_cast<RendererComponent>(add_component(new RendererComponent()));
}

void BaseRunner::awake() {
    GameObject::awake();

    if (sprite_ != nullptr)
        delete sprite_;
    sprite_ = create_sprite();

    renderer_component_->init(position_component_.get(), sprite_, Layer::LEVEL);
}

void BaseRunner::destroy() {
    if (!is_destroyed() && sprite_ != nullptr) {
        renderer_component_->clear();
        delete sprite_;
        sprite_ = nullptr;
    }
    renderer_component_->destroy();
    renderer_component_ = nullptr;
    position_component_->destroy();
    position_component_ = nullptr;
    GameObject::destroy();
}

int BaseRunner::get_start_x() {
    return start_x_;
}

int BaseRunner::get_x() {
    return position_component_->get_position().x;
}

int BaseRunner::get_lap_index() {
    return lap_index_;
}

int BaseRunner::get_laps_count() {
    return laps_count_;
}

void BaseRunner::start_lap(int lap_index, int laps_count) {
    lap_index_ = lap_index;
    laps_count_ = laps_count;
}

void BaseRunner::set_x(int x) {
    position_component_->set_position(x, y_);
}
