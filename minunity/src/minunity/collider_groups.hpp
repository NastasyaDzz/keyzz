#ifndef COLLIDER_GROUPS_HPP_INCLUDED
#define COLLIDER_GROUPS_HPP_INCLUDED

#include <array>
#include <map>
#include <memory>
#include <vector>

#include "group_id.hpp"
#include "collider_component.hpp"

namespace minunity {

struct CollisionInfo {
    ColliderComponent* collider = nullptr;
    bool in_touch = false;
    bool has_intersection = false;
    bool not_equal(CollisionInfo& other) {
        return collider != other.collider ||
            in_touch != other.in_touch ||
            has_intersection != other.has_intersection;
    }
};

class ColliderGroups {
 public:
    void add_pair(group_id first, group_id second);
    void remove_pair(group_id first, group_id second);
    void clear();

    void prepare(const std::vector<std::shared_ptr<ColliderComponent>>& colliders);
    
    std::vector<std::vector<bool>>& get_collision_matrix() { return collision_matrix_; }

    std::map<int, std::vector<std::shared_ptr<ColliderComponent>>>& get_map() { return map_; }

    std::map<ColliderComponent*, CollisionInfo> collider_collisions_;

    void check_collisions(ColliderComponent* collider);
 private:
    std::vector<std::vector<bool>> collision_matrix_ {{
        {0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0}
    }};
    std::map<int, std::vector<std::shared_ptr<ColliderComponent>>> map_ {
        {0, {}},
        {1, {}},
        {2, {}},
        {3, {}},
        {4, {}},
        {5, {}}
    };
};

}

#endif // COLLIDER_GROUPS_HPP_INCLUDED
