// Copyright 2020 DmitryDzz

#include "collider_component.hpp"

#include <logger/easylogging++.h>

using minunity::ColliderComponent;
using minunity::group_id;

ColliderComponent::ColliderComponent(
        group_id grp_id,
        PositionComponent &position_component,
        Rect local_rect,
        IColliderCallback *callback) :
    Component(),
    group_id_(grp_id),
    position_component_(position_component),
    local_rect_(local_rect),
    callback_(callback) {
}

group_id& ColliderComponent::get_group_id() {
    return group_id_;
}

void ColliderComponent::get_rect(Rect &rect) {
    rect.x = position_component_.get_position().x + local_rect_.x;
    rect.y = position_component_.get_position().y + local_rect_.y;
    rect.w = local_rect_.w;
    rect.h = local_rect_.h;
}

void ColliderComponent::set_rect(Rect rect) {
    local_rect_ = rect;
}

void ColliderComponent::invoke_collision(ColliderComponent &other, bool touch, bool intersection) {
    if (callback_ != nullptr) {
        callback_->on_collision(*this, other, touch, intersection);
    }
}

bool ColliderComponent::has_callback() {
    return callback_ != nullptr;
}

void ColliderComponent::check_collision(Rect &r1, Rect &r2, bool* in_touch, bool* has_intersection) {
    *in_touch = false;
    *has_intersection = false;

    // r1 points:
    const Point p1(r1.x, r1.y);
    const Point p2(r1.x + r1.w - 1, r1.y);
    const Point p3(r1.x + r1.w - 1, r1.y + r1.h - 1);
    const Point p4(r1.x, r1.y + r1.h - 1);

    // r2 intervals:
    const int x1 = r2.x - 1;
    const int x2 = r2.x + r2.w;
    const int y1 = r2.y - 1;
    const int y2 = r2.y + r2.h;

    // At least one r1 vertex is inside r2 intervals:
    const Point points[] = { p1, p2, p3, p4 };
    for (int i = 0; i < 4; i++) {
        Point p = points[i];
        if (!(*in_touch) && p.x >= x1 && p.x <= x2 && p.y >= y1 && p.y <= y2)
            *in_touch = true;
        if (!(*has_intersection) && p.x > x1 && p.x < x2 && p.y > y1 && p.y < y2)
            *has_intersection = true;
    }

    // r2 is inside r1:
    if (!(*has_intersection) && r2.x >= r1.x && r2.x < r1.x + r1.w && r2.y >= r1.y && r2.y < r1.y + r1.h)
        *has_intersection = true;
}

bool ColliderComponent::in_touch(Rect &r1, Rect &r2) {
    bool in_touch;
    bool has_intersection;
    check_collision(r1, r2, &in_touch, &has_intersection);
    return in_touch;
}

bool ColliderComponent::has_intersection(Rect &r1, Rect &r2) {
    bool in_touch;
    bool has_intersection;
    check_collision(r1, r2, &in_touch, &has_intersection);
    return has_intersection;
}
