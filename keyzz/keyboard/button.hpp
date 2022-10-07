#ifndef KEYZZ_BUTTON_HPP_INCLUDED
#define KEYZZ_BUTTON_HPP_INCLUDED

#include <memory>
#include <optional>

#include <minunity/game_object.hpp>
#include <minunity/position_component.hpp>
#include <minunity/renderer_component.hpp>
#include <minunity/animation_component.hpp>
#include <minunity/animator_component.hpp>
#include <minunity/sprite.hpp>

namespace keyzz {

class Button;

class IButtonCallback {
public:
    virtual void before_pressed(Button& sender, const int key_code, bool* proceed) = 0;
    virtual void after_pressed(Button& sender, const int key_code) = 0;
};

class ShiftButton;

class Button : public minunity::GameObject {
public:
    Button(int x, int y, int w,
           std::wstring text, std::wstring pressed_text, std::wstring pressed_shift_text,
           std::optional<int> key_code, std::optional<int> shift_key_code,
           IButtonCallback *pressed_callback, std::shared_ptr<ShiftButton> shift_button = nullptr,
           std::optional<char> color_pair_index = std::nullopt);
    void awake() override;
    void destroy() override;
    void update() override;
protected:
    int get_w() { return w_; }
    std::wstring get_text() { return text_; }
    std::wstring get_pressed_text() { return pressed_text_; }
    std::wstring get_pressed_shift_text() { return pressed_shift_text_; }

    virtual minunity::Sprite* create_sprite();
    minunity::Sprite* get_sprite() { return this->sprite_; }
    std::shared_ptr<minunity::PositionComponent> position_component_ = nullptr;
    std::shared_ptr<minunity::RendererComponent> renderer_component_ = nullptr;
    std::shared_ptr<minunity::AnimationComponent> idle_animation_ = nullptr;
    std::shared_ptr<minunity::AnimationComponent> pressed_animation_ = nullptr;
    std::shared_ptr<minunity::AnimationComponent> shift_pressed_animation_ = nullptr;
    std::shared_ptr<minunity::AnimatorComponent> animator_ = nullptr;
private:
    const int MIN_W = 5;
    int x_, y_, w_;
    std::wstring text_;
    std::wstring pressed_text_;
    std::wstring pressed_shift_text_;
    std::optional<int> key_code_;
    std::optional<int> shift_key_code_;
    IButtonCallback *button_callback_;
    std::shared_ptr<ShiftButton> shift_button_;
    std::optional<char> color_pair_index_;
    minunity::Sprite *sprite_ = nullptr;
    std::wstring build_frame(
            std::wstring left_top, std::wstring right_top,
            std::wstring left_bottom, std::wstring right_bottom,
            std::wstring horizontal, std::wstring vertical,
            std::wstring text);
};

}

#endif // KEYZZ_BUTTON_HPP_INCLUDED
