#ifndef START_DIALOG_HPP_INCLUDED
#define START_DIALOG_HPP_INCLUDED

#include <memory>
#include <optional>

#include <minunity/game_object.hpp>
#include <minunity/position_component.hpp>
#include <minunity/renderer_component.hpp>
#include <minunity/scene.hpp>
#include <minunity/sprite.hpp>

#include "dialog.hpp"
#include "../keyboard/button.hpp"

namespace keyzz {

class IStartDialogCallback {
public:
    virtual void on_play() = 0;
    virtual void on_quit() = 0;
};

class StartDialog : public Dialog {
public:
    StartDialog(minunity::Scene *scene, IStartDialogCallback *dialog_callback);
    void set_visible(bool visible) override;
protected:
    minunity::Sprite* create_sprite() override;
    void button_handler(int64_t button_id) override;
private:
    IStartDialogCallback *dialog_callback_;
    std::shared_ptr<Button> play_btn_ = nullptr;
    std::shared_ptr<Button> quit_btn_ = nullptr;
};

}

#endif  // START_DIALOG_HPP_INCLUDED
