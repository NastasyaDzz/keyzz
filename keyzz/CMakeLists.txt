add_executable(keyzz
    main.cpp
    app_manager.cpp
    scenes/scene_level_1.cpp
    keyboard/button.cpp
    keyboard/shift_button.cpp
    keyboard/keyboard.cpp
    text_block/text_block.cpp
    tracks/track.cpp
    tracks/race.cpp
    runners/base_runner.cpp
    runners/my_runner.cpp
    runners/runner.cpp
    dialogs/label.cpp
    dialogs/dialog.cpp
    dialogs/start_dialog.cpp
    dialogs/finish_dialog.cpp
    dialogs/continue_button.cpp
)

target_link_libraries(keyzz PRIVATE logger minunity ${CURSES_LIBRARIES})
