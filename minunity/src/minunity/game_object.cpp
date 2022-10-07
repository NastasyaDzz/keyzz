// Copyright 2020 DmitryDzz

#include "game_object.hpp"

#include "component.hpp"
#include "scene.hpp"

using minunity::Component;
using minunity::GameObject;
using minunity::Scene;

int s_obj_id = 0;

GameObject::GameObject() {
    id_ = ++s_obj_id;
}

GameObject::~GameObject() {
    if (!destroyed_)
        destroy();
}

void GameObject::awake() {
}

void GameObject::start() {
}

void GameObject::destroy() {
    destroyed_ = true;
//    components_.clear();
}

void GameObject::update() {
    if (destroyed_) return;
    auto i = std::begin(components_);
    while (i != std::end(components_)) {
        Component *component = (*i).get();
        if (component->is_destroyed()) {
            components_.erase(i);
        } else {
            if (dynamic_cast<RendererComponent*>(component) == nullptr) {  // (render latter)
                component->update();
            }
            i++;
        }
    }
}

void GameObject::render_layer(Layer layer) {
    auto i = std::begin(components_);
    while (i != std::end(components_)) {
        Component* component = (*i).get();
        if (component->is_destroyed()) {
            components_.erase(i);
        } else {
            RendererComponent* renderer = dynamic_cast<RendererComponent*>(component);
            if (renderer && renderer->get_layer() == layer) {  // (RendererComponents only)
                renderer->update();
            }
            i++;
        }
    }
}

void GameObject::redraw() {
    auto i = std::begin(components_);
    while (i != std::end(components_)) {
        Component* component = (*i).get();
        if (component->is_destroyed()) {
            components_.erase(i);
        } else {
            RendererComponent* renderer = dynamic_cast<RendererComponent*>(component);
            if (renderer) {  // (RendererComponents only)
                renderer->redraw();
            }
            i++;
        }
    }
}

void GameObject::set_scene(Scene *scene) {
    scene_ = scene;
}

Scene* GameObject::get_scene() {
    return scene_;
}

std::shared_ptr<Component> GameObject::add_component(Component *component) {
    std::shared_ptr<Component> shared_component(component);
    components_.push_back(shared_component);
    shared_component->set_game_object(this);

    std::shared_ptr<RendererComponent> sh_renderer_component =
        std::dynamic_pointer_cast<RendererComponent>(shared_component);
    if (sh_renderer_component != nullptr) {  // Component is a RendererComponent
        renderer_component_ = sh_renderer_component;
        renderer_component_->set_visible(visible_);
    }

    return shared_component;
}

void GameObject::set_active(bool active) {
    active_ = active;
}

bool GameObject::get_active() {
    return active_;
}

void GameObject::set_visible(bool visible) {
    visible_ = visible;
    if (renderer_component_ != nullptr)
        renderer_component_->set_visible(visible);
}

bool GameObject::get_visible() {
    return visible_;
}
