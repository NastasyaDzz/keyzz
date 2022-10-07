set(CURSES_NEED_NCURSES TRUE)
set(CURSES_NEED_WIDE TRUE)
find_package(Curses REQUIRED)

add_library(
    minunity
    src/minunity/engine.cpp
    src/minunity/input.cpp
    src/minunity/scene.cpp
    src/minunity/component.cpp
    src/minunity/position_component.cpp
    src/minunity/game_object.cpp
    src/minunity/game_exception.cpp
    src/minunity/graph.cpp
    src/minunity/time.cpp
    src/minunity/sprite.cpp
    src/minunity/renderer_component.cpp
    src/minunity/animation_component.cpp
    src/minunity/animator_component.cpp
    src/minunity/group_id.cpp
    src/minunity/collider_component.cpp
    src/minunity/collider_groups.cpp
    )

target_include_directories(minunity PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")
target_include_directories(minunity PRIVATE ${CURSES_INCLUDE_DIR})
target_link_libraries(minunity PRIVATE logger ${CURSES_LIBRARIES})
