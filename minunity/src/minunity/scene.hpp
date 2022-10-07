#ifndef SCENE_HPP_INCLUDED
#define SCENE_HPP_INCLUDED

#include <memory>
#include <vector>

#include "collider_component.hpp"
#include "renderer_component.hpp"

namespace minunity {

class GameObject;

class Scene {
public:
    virtual void awake();
    virtual void start();
    virtual void destroy();
    virtual void update();

    void render();
    void redraw();

    virtual std::shared_ptr<GameObject> add_game_object(GameObject *go);
    std::vector<std::shared_ptr<GameObject>>& get_game_objects() { return game_objects_; }

    void update_colliders_list();
    const std::vector<std::shared_ptr<ColliderComponent>>& get_colliders() { return colliders_; }
private:
    std::vector<std::shared_ptr<GameObject>> game_objects_;
    std::vector<std::shared_ptr<ColliderComponent>> colliders_;
    void process_collisions();
    void render_layer(Layer layer);
};

}

#endif // SCENE_HPP_INCLUDED
