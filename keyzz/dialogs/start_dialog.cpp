// Copyright 2021 DmitryDzz

#include "start_dialog.hpp"

#include <string>

#include <minunity/engine.hpp>

using minunity::Engine;
using minunity::Layer;
using minunity::PositionComponent;
using minunity::RendererComponent;
using minunity::Sprite;

using keyzz::StartDialog;

StartDialog::StartDialog(minunity::Scene *scene, IStartDialogCallback *dialog_callback)
    : Dialog(25, 1), dialog_callback_(dialog_callback) {
    std::wstring caption = L" (P)lay ";
    play_btn_ = std::dynamic_pointer_cast<Button>(scene->add_game_object(
                new Button(30, 5, 10, caption, caption, caption, 'p', 'P', this)));
    play_btn_->set_active(false);

    caption = L" (Q)uit ";
    quit_btn_ = std::dynamic_pointer_cast<Button>(scene->add_game_object(
                new Button(41, 5, 10, caption, caption, caption, 'q', 'Q', this)));
    quit_btn_->set_active(false);
}

Sprite* StartDialog::create_sprite() {
    Sprite *result = new Sprite(31, 8, 1);
    std::wstring s = L"";
    //               1         2         3
    //     0123456789012345678901234567890
    s += L"╔═════════════════════════════╗";  // 0
    s += L"║                             ║";  // 1
    s += L"║        Are you ready?       ║";  // 2
    s += L"║                             ║";  // 3
    s += L"║    ┌────────┐ ┌────────┐    ║";  // 4
    s += L"║    │ (P)lay │ │ (Q)uit │    ║";  // 5
    s += L"║    └────────┘ └────────┘    ║";  // 6
    s += L"╚═════════════════════════════╝";  // 7
    result->load(s.c_str());
    return result;
}

void StartDialog::set_visible(bool visible) {
    Dialog::set_visible(visible);
    play_btn_->set_active(visible);
    quit_btn_->set_active(visible);
}

void StartDialog::button_handler(int64_t button_id) {
    set_visible(false);
    if (dialog_callback_) {
        if (button_id == play_btn_->get_id())
            dialog_callback_->on_play();
        else if (button_id == quit_btn_->get_id())
            dialog_callback_->on_quit();
    }
}
