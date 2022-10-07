// Copyright 2020 DmitryDzz

#include "collider_groups.hpp"

#include <logger/easylogging++.h>

#include "game_object.hpp"
#include "graph.hpp"

using minunity::ColliderComponent;
using minunity::ColliderGroups;
using minunity::Rect;

void ColliderGroups::add_pair(group_id first, group_id second) {
    if (first == second) return;
    collision_matrix_.at(first.value()).at(second.value()) = true;
    collision_matrix_.at(second.value()).at(first.value()) = true;
}

void ColliderGroups::remove_pair(group_id first, group_id second) {
    collision_matrix_.at(first.value()).at(second.value()) = false;
    collision_matrix_.at(second.value()).at(first.value()) = false;
}

void ColliderGroups::clear() {
    for (auto& vector : collision_matrix_) {
        std::fill(vector.begin(), vector.end(), false);
    }
}

void ColliderGroups::prepare(const std::vector<std::shared_ptr<ColliderComponent>>& colliders) {
    for (auto& imap : map_) {
        std::vector<std::shared_ptr<ColliderComponent>>& group_colliders = imap.second;
        group_colliders.clear();
        const std::vector<bool>& collisions = collision_matrix_[imap.first];
        for (const auto& collider : colliders) {
            if (collisions[collider.get()->get_group_id().value()]) {
                group_colliders.push_back(collider);
            }
        }
    }
}

void ColliderGroups::check_collisions(ColliderComponent* collider) {
    if (collider->get_game_object()->is_destroyed()) return;
    if (!collider->has_callback() || !collider->get_enabled()) return;
    if (!collider->get_game_object()->get_active()) {
        collider_collisions_.erase(collider);
        return;
    }
    int max_group_id = collision_matrix_.size() - 1;
    if (collider->get_group_id() < 0 || collider->get_group_id() > max_group_id) return;
    std::vector<std::shared_ptr<ColliderComponent>> other_colliders = map_[collider->get_group_id().value()];
    Rect r1;
    Rect r2;

    CollisionInfo* p_prev = nullptr;
    if (collider_collisions_.count(collider))
        p_prev = &collider_collisions_[collider];

    for (auto& sh_other : other_colliders) {
        ColliderComponent* current_other = sh_other.get();
        if (current_other->get_game_object()->is_destroyed()) continue;
        if (!current_other->get_game_object()->get_active()) continue;
        if (!current_other->get_enabled()) continue;
        if ((current_other->get_group_id() < 0) || (current_other->get_group_id() > max_group_id)) continue;
        collider->get_rect(r1);
        current_other->get_rect(r2);

//        LOG(INFO) << "[ColliderGroups] c" << collider->get_id() <<
//            "  r1:" << r1.x << "," << r1.y << "," << r1.w << "," << r1.h <<
//            "  current_other" << current_other->get_id() <<
//            "  r2:" << r2.x << "," << r2.y << "," << r2.w << "," << r2.h;

        bool current_in_touch;
        bool current_has_intersection;
        ColliderComponent::check_collision(r1, r2, &current_in_touch, &current_has_intersection);
        if (current_in_touch || current_has_intersection) {
            CollisionInfo current {
                .collider = current_other,
                .in_touch = current_in_touch,
                .has_intersection = current_has_intersection
            };

            if (p_prev == nullptr) {
                p_prev = &current;
                collider_collisions_[collider] = current;
                collider->invoke_collision(*current.collider, current_in_touch, current_has_intersection);
                break;
            } else {
                if (current.not_equal(*p_prev)) {
                    collider_collisions_[collider] = current;
                    collider->invoke_collision(*current.collider, current_in_touch, current_has_intersection);
                }
            }
        }
    }

    if (p_prev == nullptr) {
        collider_collisions_[collider] = CollisionInfo {
            .collider = nullptr,
            .in_touch = false,
            .has_intersection = false
        };
    }
}
