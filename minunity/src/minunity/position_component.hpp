#ifndef POSITION_COMPONENT_HPP_INCLUDED
#define POSITION_COMPONENT_HPP_INCLUDED

#include "component.hpp"
#include "graph.hpp"

namespace minunity {

class PositionComponent : public Component {
public:
    virtual void set_position(minunity::Point& position);
    virtual void set_position(int x, int y);
    virtual minunity::Point& get_position();

    bool get_modified() { return this->is_modified_; }
    void clear_modified() { this->is_modified_ = false; }
private:
    minunity::Point previous_position_ = { -1, -1 };
    minunity::Point position_ = { 0, 0 };
    bool is_modified_ = true;
    void check_modified();
};

}

#endif // POSITION_COMPONENT_HPP_INCLUDED
