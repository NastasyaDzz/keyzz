#ifndef KEYZZ_LABEL_HPP_INCLUDED
#define KEYZZ_LABEL_HPP_INCLUDED

#include <memory>

#include <minunity/game_object.hpp>
#include <minunity/graph.hpp>
#include <minunity/position_component.hpp>
#include <minunity/renderer_component.hpp>
#include <minunity/sprite.hpp>

namespace keyzz {

enum class LabelAlignment {
    left,
    right
};

class Label : public minunity::GameObject {
public:
    Label(int x, int y, int w, std::wstring text, LabelAlignment alignment = LabelAlignment::left);
    void awake() override;
    void destroy() override;
    void render_layer(minunity::Layer layer) override;

    std::wstring get_text();
    void set_text(std::wstring text);

    void set_position(minunity::Point& position);
    void set_position(int x, int y);
    minunity::Point& get_position();
private:
    const int MIN_W = 1;
    int x_, y_, w_;
    std::wstring text_;
    LabelAlignment alignment_;
    minunity::Sprite *sprite_ = nullptr;
    std::shared_ptr<minunity::PositionComponent> position_component_ = nullptr;
    std::shared_ptr<minunity::RendererComponent> renderer_component_ = nullptr;
};

}

#endif // KEYZZ_LABEL_HPP_INCLUDED
