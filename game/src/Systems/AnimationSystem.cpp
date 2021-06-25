#include "Systems/AnimationSystem.hpp"



void AnimationSystem::update(float) {
    auto currentTime = ::std::chrono::duration_cast<::std::chrono::milliseconds>(::std::chrono::system_clock::now().time_since_epoch());

    auto entities = getEntities();
    for (auto &entity : entities) {
        auto &animComponent = entity.getComponent<AnimationComponent>();
        if (animComponent.state == AnimationComponent::Start) {
            animComponent.instance->activate();
            animComponent.startTime = ::std::chrono::duration_cast<::std::chrono::milliseconds>(::std::chrono::system_clock::now().time_since_epoch());
            animComponent.endTime = animComponent.startTime + animComponent.animTime;
            animComponent.state = AnimationComponent::Play;
        }
        if (currentTime >= animComponent.endTime) {
            entity.deactivate();
            animComponent.instance->deactivate();
            animComponent.state = AnimationComponent::Start;
        }
        else if (currentTime - animComponent.startTime >= animComponent.frameDuration) {
            animComponent.startTime = ::std::chrono::duration_cast<::std::chrono::milliseconds>(::std::chrono::system_clock::now().time_since_epoch());
            animComponent.currentFrame = (animComponent.currentFrame + 1) % animComponent.frames.size();
            animComponent.instance->getComponent<Sprite>().setImage(animComponent.frames.at(animComponent.currentFrame));
        }
    }
}
