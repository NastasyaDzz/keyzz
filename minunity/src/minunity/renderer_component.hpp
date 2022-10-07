#ifndef RENDERER_COMPONENT_HPP_INCLUDED
#define RENDERER_COMPONENT_HPP_INCLUDED

#include "component.hpp"
#include "position_component.hpp"
#include "sprite.hpp"

#include <optional>

#include <ncurses.h>

namespace minunity {

enum class Layer {
    BACKGROUND = 0,
    LEVEL,
    ENEMY,
    PROPS,
    BULLET,
    PLAYER,
    GUI_BACKGROUND,
    GUI_FOREGROUND,
    RESERVED
};

class RendererComponent : public Component {
public:
    void init(PositionComponent *positionComponent, Sprite *sprite, Layer layer);
    void set_color_pair_index(char color_pair_index);

    void update() override;
    void destroy() override;
    void clear();

    void set_visible(bool visible);
    bool get_visible();
    void skip_frame() { skip_frame_ = true; }

    Layer get_layer() { return layer_; }
    void set_layer(Layer layer) { layer_ = layer; }

    void redraw();
private:
    PositionComponent *position_component_ = nullptr;
    Sprite *sprite_ = nullptr;
    WINDOW *win = nullptr;
    bool win_has_content = false;
    bool visible_ = true;
    bool visibility_modified_ = false;
    bool force_redraw_ = false;
    Layer layer_ = Layer::BACKGROUND;
    bool skip_frame_ = false;
    std::optional<char> color_pair_index_ = std::nullopt;
};

}

#endif // RENDERER_COMPONENT_HPP_INCLUDED
