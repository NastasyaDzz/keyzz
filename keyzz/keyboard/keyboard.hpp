#ifndef KEYBOARD_HPP_INCLUDED
#define KEYBOARD_HPP_INCLUDED

#include <minunity/game_object.hpp>
#include <minunity/scene.hpp>

#include "button.hpp"

namespace keyzz {

class IKeyboardCallback {
public:
    virtual void on_key_pressed(Button& sender, const int key_code) = 0;
};

class Keyboard : public minunity::GameObject, private IButtonCallback {
public:
    Keyboard(minunity::Scene *scene, IKeyboardCallback *keyboard_callback);
private:
    minunity::Scene *scene_;
    IKeyboardCallback *keyboard_callback_;
    void build();
    void before_pressed(Button& sender, const int key_code, bool* proceed) override;
    void after_pressed(Button& sender, const int key_code) override;
};

}

#endif // KEYBOARD_HPP_INCLUDED
