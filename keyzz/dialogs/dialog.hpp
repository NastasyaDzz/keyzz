#ifndef DIALOG_HPP_INCLUDED
#define DIALOG_HPP_INCLUDED

#include <memory>
#include <optional>

#include <minunity/game_object.hpp>
#include <minunity/position_component.hpp>
#include <minunity/renderer_component.hpp>
#include <minunity/scene.hpp>
#include <minunity/sprite.hpp>

#include "../keyboard/button.hpp"

namespace keyzz {

class Dialog : public minunity::GameObject, protected IButtonCallback {
public:
    Dialog(int x, int y);
    void awake() override;
    void update() override;
    void set_visible(bool visible) override;
    void show();
    void hide();
protected:
    std::shared_ptr<minunity::PositionComponent> position_component_ = nullptr;
    std::shared_ptr<minunity::RendererComponent> renderer_component_ = nullptr;
    minunity::Scene *get_scene();
    virtual minunity::Sprite* create_sprite() = 0;
    virtual void before_pressed(Button& sender, const int key_code, bool* proceed);
    virtual void after_pressed(Button& sender, const int key_code);
    virtual void button_handler(long button_id) = 0;
private:
    minunity::Sprite *sprite_ = nullptr;
    std::optional<long> selected_btn_id_ = std::nullopt;
    std::optional<long> action_millis_ = std::nullopt;
};

}

#endif  // DIALOG_HPP_INCLUDED
