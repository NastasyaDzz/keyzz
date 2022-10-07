#ifndef ANIMATOR_COMPONENT_HPP_INCLUDED
#define ANIMATOR_COMPONENT_HPP_INCLUDED

#include <map>
#include <memory>

#include "component.hpp"
#include "animation_component.hpp"

namespace minunity {

class GameObject;

class AnimatorComponent : public Component {
public:
    int add_animation(std::shared_ptr<AnimationComponent> animation);
    void remove_animation(int component_id);
    void clear();

    void stop();
    void play(int component_id,
            IAnimationStopped* on_stopped = nullptr,
            IBeforeAnimationFrame* before_animation_frame = nullptr);
private:
    std::map<int, std::shared_ptr<AnimationComponent>> animations_;
};

}

#endif // ANIMATOR_COMPONENT_HPP_INCLUDED
