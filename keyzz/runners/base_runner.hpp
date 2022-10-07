#ifndef BASE_RUNNER_HPP_INCLUDED
#define BASE_RUNNER_HPP_INCLUDED

#include <memory>

#include <minunity/game_object.hpp>
#include <minunity/position_component.hpp>
#include <minunity/renderer_component.hpp>
#include <minunity/sprite.hpp>

#include "../tracks/track.hpp"

namespace keyzz {

class BaseRunner : public minunity::GameObject {
public:
    BaseRunner(int x, int y, std::shared_ptr<Track> track_record);
    void awake() override;
    void destroy() override;
    int get_start_x();
    int get_x();
    void set_x(int x);

    virtual void start_lap(int lap_index, int laps_count);
    int get_lap_index();
    int get_laps_count();
private:
    int start_x_;
    int y_;
    int lap_index_;
    int laps_count_;
    minunity::Sprite *sprite_ = nullptr;
protected:
    std::shared_ptr<Track> track_;
    virtual minunity::Sprite* create_sprite() = 0;
    minunity::Sprite* get_sprite() { return this->sprite_; }
    std::shared_ptr<minunity::PositionComponent> position_component_ = nullptr;
    std::shared_ptr<minunity::RendererComponent> renderer_component_ = nullptr;
};

}

#endif // BASE_RUNNER_HPP_INCLUDED
