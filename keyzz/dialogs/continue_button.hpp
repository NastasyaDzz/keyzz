#ifndef CONTINUE_BUTTON_HPP_INCLUDED
#define CONTINUE_BUTTON_HPP_INCLUDED

#include <memory>

#include <minunity/game_object.hpp>
#include <minunity/scene.hpp>
#include <minunity/sprite.hpp>

#include "../keyboard/button.hpp"

namespace keyzz {

class IContinueButtonPressedCallback {
public:
    virtual void on_continue_button_pressed(Button& sender) = 0;
};

class TextButton : public Button {
public:
    TextButton(int x, int y, int w,
           std::wstring text, std::wstring pressed_text, std::wstring pressed_shift_text,
           std::optional<int> key_code, std::optional<int> shift_key_code,
           IButtonCallback *pressed_callback, std::shared_ptr<ShiftButton> shift_button = nullptr,
           std::optional<char> color_pair_index = std::nullopt);
protected:
    minunity::Sprite* create_sprite() override;
};

class ContinueButton : public minunity::GameObject, private IButtonCallback {
public:
    ContinueButton(minunity::Scene *scene, IContinueButtonPressedCallback *continue_callback);
    void set_active(bool active) override;
private:
    minunity::Scene *scene_;
    IContinueButtonPressedCallback *continue_callback_;
    std::shared_ptr<TextButton> button_ = nullptr;
    void build();
    void before_pressed(Button& sender, const int key_code, bool* proceed) override;
    void after_pressed(Button& sender, const int key_code) override;
};

}

#endif // CONTINUE_BUTTON_HPP_INCLUDED
