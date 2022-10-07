#ifndef COLLIDER_COMPONENT_HPP_INCLUDED
#define COLLIDER_COMPONENT_HPP_INCLUDED

#include "component.hpp"
#include "graph.hpp"
#include "position_component.hpp"
#include "group_id.hpp"

namespace minunity {

class ColliderComponent;

class IColliderCallback {
public:
    virtual void on_collision(
            ColliderComponent& this_collider,
            ColliderComponent& other_collider,
            const bool touch,
            const bool intersection) = 0;
};

class ColliderComponent : public Component {
public:
    ColliderComponent(
            group_id grp_id,
            PositionComponent &position_component,
            Rect local_rect,
            IColliderCallback *callback = nullptr);

    bool get_enabled() { return enabled_; }
    void set_enabled(bool enabled) { enabled_ = enabled; }

    group_id& get_group_id();
    void get_rect(Rect &rect);
    void set_rect(Rect rect);
    void invoke_collision(ColliderComponent &other, bool touch, bool intersection);
    bool has_callback();
    
    static bool in_touch(Rect &r1, Rect &r2);
    static bool has_intersection(Rect &r1, Rect &r2);
    static void check_collision(Rect &r1, Rect &r2, bool* in_touch, bool* has_intersection);
private:
    bool enabled_ = true;
    group_id group_id_;
    PositionComponent &position_component_;
    Rect local_rect_;
    IColliderCallback *callback_;
};

}

#endif // COLLIDER_COMPONENT_HPP_INCLUDED
