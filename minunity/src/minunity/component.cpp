// Copyright 2020 DmitryDzz

#include "component.hpp"

#include "game_object.hpp"

using minunity::Component;
using minunity::GameObject;

int s_id = 0;

Component::Component() {
    id_ = ++s_id;
    destroyed_ = false;
}

Component::~Component() {
    if (!this->destroyed_)
        this->destroy();
}

void Component::destroy() {
    this->destroyed_ = true;
}

int Component::get_id() const {
    return id_;
}

bool Component::is_destroyed() const {
    return destroyed_;
}

void Component::set_game_object(GameObject *game_object) {
    game_object_ = game_object;
}

GameObject* Component::get_game_object() {
    return game_object_;
}
