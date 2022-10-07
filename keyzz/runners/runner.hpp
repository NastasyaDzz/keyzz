#ifndef RUNNER_HPP_INCLUDED
#define RUNNER_HPP_INCLUDED

#include <memory>
#include <optional>

#include <minunity/game_object.hpp>
#include <minunity/graph.hpp>
#include <minunity/sprite.hpp>

#include "base_runner.hpp"

namespace keyzz {

class Runner : public BaseRunner {
public:
    Runner(int x, int y, std::shared_ptr<Track> track_record);
    void awake() override;
    void update() override;
    void render_layer(minunity::Layer layer) override;
    void start_lap(int lap_index, int laps_count) override;
protected:
    minunity::Sprite* create_sprite() override;
private:
    std::optional<uint32_t> start_time_ = std::nullopt;
    int lap_first_index_ = 0;
    int lap_last_index_ = 0;
    void finish_race();
};

}

#endif // RUNNER_HPP_INCLUDED
