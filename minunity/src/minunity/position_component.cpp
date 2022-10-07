// Copyright 2020 DmitryDzz

#include "position_component.hpp"

#include <logger/easylogging++.h>

using minunity::PositionComponent;

void PositionComponent::set_position(minunity::Point& position) {
    position_ = position;
    check_modified();
}

void PositionComponent::set_position(int x, int y) {
    position_.set(x, y);
    check_modified();
}

minunity::Point& PositionComponent::get_position() {
    return position_;
}

void PositionComponent::check_modified() {
    if (previous_position_.x != position_.x || previous_position_.y != position_.y) {
        is_modified_ = true;
        previous_position_ = position_;
    }
}
