#ifndef GAME_OBJECT_HPP_INCLUDED
#define GAME_OBJECT_HPP_INCLUDED

#include <memory>
#include <vector>

#include "renderer_component.hpp"

namespace minunity {
    
class Component;
class RendererComponent;
class Scene;

class GameObject {
public:
    GameObject();
    virtual ~GameObject();
    virtual void awake();
    virtual void start();
    virtual void destroy();
    virtual void update();

    virtual void render_layer(Layer layer);
    virtual void redraw();

    void set_scene(Scene *scene);
    Scene* get_scene();

    long get_id() { return this->id_; }
    bool is_destroyed() { return this->destroyed_; }
    
    virtual std::shared_ptr<Component> add_component(Component *component);
    std::vector<std::shared_ptr<Component>>& get_components() { return components_; }

    virtual void set_active(bool active);
    bool get_active();

    virtual void set_visible(bool visible);
    bool get_visible();
private:
    std::vector<std::shared_ptr<Component>> components_;
    std::shared_ptr<RendererComponent> renderer_component_ = nullptr;
    long id_;
    Scene *scene_ = nullptr;
    bool destroyed_ = false;
    bool active_ = true;
    bool visible_ = true;
};

}

#endif // GAME_OBJECT_HPP_INCLUDED
