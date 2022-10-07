// Copyright 2020 DmitryDzz

#include "scene.hpp"

#include <logger/easylogging++.h>

#include <iterator>

#include "collider_groups.hpp"
#include "engine.hpp"
#include "game_object.hpp"

using minunity::ColliderComponent;
using minunity::ColliderGroups;
using minunity::Engine;
using minunity::GameObject;
using minunity::Layer;
using minunity::Scene;

void Scene::awake() {
    auto i = std::begin(game_objects_);
    while (i != std::end(game_objects_)) {
        GameObject *go = (*i).get();
        go->awake();
        if (go->is_destroyed())
            game_objects_.erase(i);
        else
            i++;
    }
}

void Scene::start() {
    auto i = std::begin(game_objects_);
    while (i != std::end(game_objects_)) {
        GameObject *go = (*i).get();
        go->start();
        if (go->is_destroyed())
            game_objects_.erase(i);
        else
            i++;
    }
}

void Scene::destroy() {
    game_objects_.clear();
}

void Scene::update() {
    auto i = std::begin(game_objects_);
    while (i != std::end(game_objects_)) {
        GameObject *go = (*i).get();
        go->update();
        if (go->is_destroyed()) {
            game_objects_.erase(i);
            update_colliders_list();
        } else {
            i++;
        }
    }
    process_collisions();
}

void Scene::render() {
    render_layer(Layer::BACKGROUND);
    render_layer(Layer::LEVEL);
    render_layer(Layer::ENEMY);
    render_layer(Layer::PROPS);
    render_layer(Layer::BULLET);
    render_layer(Layer::PLAYER);
    render_layer(Layer::GUI_BACKGROUND);
    render_layer(Layer::GUI_FOREGROUND);
    render_layer(Layer::RESERVED);
}

void Scene::render_layer(Layer layer) {
    auto i = std::begin(game_objects_);
    while (i != std::end(game_objects_)) {
        GameObject *go = (*i).get();
        if (go->is_destroyed()) {
            game_objects_.erase(i);
            update_colliders_list();
        } else {
            go->render_layer(layer);
            i++;
        }
    }
}

void Scene::redraw() {
    auto i = std::begin(game_objects_);
    while (i != std::end(game_objects_)) {
        GameObject *go = (*i).get();
        if (go->is_destroyed()) {
            game_objects_.erase(i);
            update_colliders_list();
        } else {
            go->redraw();
            i++;
        }
    }
}

std::shared_ptr<GameObject> Scene::add_game_object(GameObject *go) {
    std::shared_ptr<GameObject> shared_go(go);
    game_objects_.push_back(shared_go);
    shared_go->set_scene(this);
    return shared_go;
}

void Scene::update_colliders_list() {
    colliders_.clear();
    for (auto& sh_go : game_objects_) {
        GameObject *go = sh_go.get();
        for (auto& s_co : go->get_components()) {
            std::shared_ptr<ColliderComponent> shared_collider_component_ptr =
                std::dynamic_pointer_cast<ColliderComponent>(s_co);

            if (shared_collider_component_ptr != nullptr)  // Component is a ColliderComponent
                colliders_.push_back(shared_collider_component_ptr);
        }
    }
}

void Scene::process_collisions() {
    ColliderGroups& collider_groups = Engine::get_instance()->get_collider_groups();
    for (auto& sh_c : colliders_) {
        collider_groups.check_collisions(sh_c.get());
    }
}
