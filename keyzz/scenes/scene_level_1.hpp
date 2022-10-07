#ifndef KEYZZ_SCENE_LEVEL1_HPP_INCLUDED
#define KEYZZ_SCENE_LEVEL1_HPP_INCLUDED

#include <memory>
#include <optional>
#include <string>

#include <minunity/scene.hpp>

#include "../app_manager.hpp"
#include "../dialogs/start_dialog.hpp"
#include "../dialogs/finish_dialog.hpp"
#include "../dialogs/label.hpp"
#include "../dialogs/continue_button.hpp"
#include "../keyboard/keyboard.hpp"
#include "../keyboard/button.hpp"
#include "../text_block/text_block.hpp"
#include "../tracks/track.hpp"
#include "../tracks/race.hpp"
#include "../runners/my_runner.hpp"
#include "../runners/runner.hpp"

namespace keyzz {

class SceneLevel1 : public minunity::Scene,
    private IRaceCallback,
    private IKeyboardCallback,
    private IContinueButtonPressedCallback,
    private IStartDialogCallback,
    private IFinishDialogCallback {
public:
    SceneLevel1(std::string filename, AppManager& app_manager);
    void awake() override;
    void update() override;
private:
    std::string filename_;
    AppManager& app_manager_;
    std::shared_ptr<Race> race_;

    std::shared_ptr<keyzz::TextBlock> text_block_ = nullptr;
    std::shared_ptr<keyzz::MyRunner> my_runner_ = nullptr;
    std::vector<std::shared_ptr<keyzz::Runner>> runners_;
    std::shared_ptr<keyzz::Keyboard> keyboard_ = nullptr;
    std::shared_ptr<keyzz::Label> lap_counter_ = nullptr;
    std::shared_ptr<keyzz::Label> error_counter_ = nullptr;
    std::shared_ptr<keyzz::ContinueButton> continue_button_ = nullptr;
    
    std::shared_ptr<keyzz::StartDialog> start_dialog_ = nullptr;
    std::shared_ptr<keyzz::FinishDialog> finish_dialog_ = nullptr;
    std::optional<uint32_t> finish_dialog_show_millis_ = std::nullopt;
    bool continue_button_pressed_ = false;

    // IRaceCallback members:
    void on_lap_start(std::wstring lap_text, std::optional<std::wstring> next_lap_text, int lap_index, int laps_count);
    void on_race_finished();

    // IKeyboardCallback members:
    void on_key_pressed(Button& sender, const int key);

    // IContinueButtonPressedCallback members:
    void on_continue_button_pressed(Button& sender);

    // IStartDialogCallback and IFinishDialogCallback members:
    void on_play();
    void on_quit();

    void create_text_block();
    void create_runners();
    void create_keyboard();
    void create_dialogs();
    void create_lap_counter();
    void output_lap_counter(int lap_index, int laps_count);

    uint32_t errors_count_ = 0;
    void create_error_counter();
    void output_error_counter(uint32_t errors_count);

    // class ContinueButtonCallback : IButtonCallback {
    // public:
    //     void before_pressed(Button& sender, const int key_code, bool* proceed) override;
    //     void after_pressed(Button& sender, const int key_code) override;
    // };

    // std::shared_ptr<ContinueButtonCallback> continue_button_callbacks_ = nullptr;
    void create_continue_button();
};

}

#endif // KEYZZ_SCENE_LEVEL1_HPP_INCLUDED
