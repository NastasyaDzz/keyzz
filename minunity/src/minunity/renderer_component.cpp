// Copyright 2020 DmitryDzz

#include "renderer_component.hpp"

#include <logger/easylogging++.h>

#include "game_object.hpp"

using minunity::RendererComponent;

void RendererComponent::init(PositionComponent *position_component, Sprite *sprite, Layer layer) {
    if (win != nullptr)
        destroy();

    position_component_ = position_component;
    sprite_ = sprite;
    layer_ = layer;

    win = newwin(sprite->get_height(), sprite->get_width(),
            position_component->get_position().y, position_component->get_position().x);
    win_has_content = false;
}

void RendererComponent::set_color_pair_index(char color_pair_index) {
    color_pair_index_ = color_pair_index;
}

void RendererComponent::update() {
//    if (get_game_object()->get_id() == 2) {
//        Point p = position_component_->get_position();
//        LOG(INFO) << "[RendererComponent] +++++ pos=(" << p.x << ", " << p.y << ") skip_frame=" << skip_frame_;
//    }
    if (get_game_object()->is_destroyed()) return;
    if (skip_frame_) {
        skip_frame_ = false;
        return;
    }
    if (!get_game_object()->get_active()) {
        if (win_has_content)
            clear();
        return;
    }
    if (position_component_->get_modified() || sprite_->get_modified() || visibility_modified_ || force_redraw_) {
        force_redraw_ = false;
        if (win_has_content)
            clear();
        win_has_content = true;
        mvwin(win, position_component_->get_position().y, position_component_->get_position().x);
        if (visible_) {
            if (color_pair_index_)
                wattron(win, COLOR_PAIR(color_pair_index_.value()));
            for (int i = 0; i < sprite_->get_height(); i++) {
                mvwaddwstr(win, i, 0, sprite_->get_row(i));
            }
            if (color_pair_index_)
                wattroff(win, COLOR_PAIR(color_pair_index_.value()));
        }
        wrefresh(win);

        position_component_->clear_modified();
        sprite_->clear_modified();
        visibility_modified_ = false;
    }
}

void RendererComponent::redraw() {
    force_redraw_ = true;
}

void RendererComponent::clear() {
    if (win != nullptr && win_has_content) {
        win_has_content = false;
        werase(win);
        wrefresh(win);
    }
}

void RendererComponent::destroy() {
    clear();
    if (win != nullptr) {
        delwin(win);
        win = nullptr;
    }
    Component::destroy();
}

void RendererComponent::set_visible(bool visible) {
    if (visible == visible_) return;
    visible_ = visible;
    visibility_modified_ = true;
//    if (win != nullptr) update();
}

bool RendererComponent::get_visible() {
    return visible_;
}
