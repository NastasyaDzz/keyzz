#ifndef FINISH_DIALOG_HPP_INCLUDED
#define FINISH_DIALOG_HPP_INCLUDED

#include <memory>
#include <optional>
#include <vector>

#include <minunity/game_object.hpp>
#include <minunity/position_component.hpp>
#include <minunity/renderer_component.hpp>
#include <minunity/scene.hpp>
#include <minunity/sprite.hpp>

#include "dialog.hpp"
#include "../keyboard/button.hpp"
#include "label.hpp"
#include "../tracks/race.hpp"

namespace keyzz {

class IFinishDialogCallback {
public:
    virtual void on_play() = 0;
    virtual void on_quit() = 0;
};

class FinishDialog : public Dialog {
public:
    FinishDialog(minunity::Scene *scene, IFinishDialogCallback *dialog_callback, std::shared_ptr<Race> race);
    void awake() override;
    void set_visible(bool visible) override;
protected:
    minunity::Sprite* create_sprite() override;
    void button_handler(int64_t button_id) override;
private:
    IFinishDialogCallback *dialog_callback_;
    std::shared_ptr<Race> race_;

    std::shared_ptr<Button> play_btn_ = nullptr;
    std::shared_ptr<Button> quit_btn_ = nullptr;

    std::shared_ptr<Label> title_label_ = nullptr;
    std::shared_ptr<Label> position_label_ = nullptr;
    std::vector<std::shared_ptr<Label>> record_labels_;
    void draw_race_results();
};

}

#endif  // FINISH_DIALOG_HPP_INCLUDED
