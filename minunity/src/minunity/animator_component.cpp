// Copyright 2020 DmitryDzz

#include "animator_component.hpp"

#include <logger/easylogging++.h>

using minunity::AnimatorComponent;

int AnimatorComponent::add_animation(std::shared_ptr<AnimationComponent> animation) {
    animations_[animation->get_id()] = animation;
    return animation->get_id();
}

void AnimatorComponent::remove_animation(int component_id) {
    animations_.erase(component_id);
}

void AnimatorComponent::clear() {
    animations_.clear();
}

void AnimatorComponent::stop() {
    for (auto const& item : animations_)
        item.second->stop();
}

void AnimatorComponent::play(int component_id,
        IAnimationStopped* on_stopped,
        IBeforeAnimationFrame* before_animation_frame) {
    stop();
    if (animations_.count(component_id))
        animations_[component_id]->play(on_stopped, before_animation_frame);
}
