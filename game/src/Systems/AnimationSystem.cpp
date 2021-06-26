#include "Systems/AnimationSystem.hpp"



void AnimationSystem::update(float) {
    auto currentTime = ::std::chrono::duration_cast<::std::chrono::milliseconds>(::std::chrono::system_clock::now().time_since_epoch());

    auto entities = getEntities();
    for (auto &entity : entities) {
        if (!entity.hasComponent<Sprite>()) {
            entity.addComponent<Sprite>();
        }

        auto &animComponent = entity.getComponent<AnimationComponent>();

        if (animComponent.state != AnimationComponent::Stop) {
            auto &spriteComponent = entity.getComponent<Sprite>();

            if (animComponent.state == AnimationComponent::Start) {
                animComponent.startTime = ::std::chrono::duration_cast<::std::chrono::milliseconds>(::std::chrono::system_clock::now().time_since_epoch());
                animComponent.endTime = animComponent.startTime + animComponent.animTime;
                animComponent.state = AnimationComponent::Play;

                auto &[ image, transform, color ] = animComponent.frames.at(animComponent.currentFrame);

                spriteComponent.setImage(image);
                spriteComponent.setColor(color);
//            spriteComponent.setTransform(transform);
            }
            if (currentTime >= animComponent.endTime) {
                if (animComponent.isLoop)
                    entity.deactivate();
                animComponent.state = AnimationComponent::Stop;
            }
            else if (currentTime - animComponent.startTime >= animComponent.frameDuration) {
                animComponent.startTime = ::std::chrono::duration_cast<::std::chrono::milliseconds>(::std::chrono::system_clock::now().time_since_epoch());
                animComponent.currentFrame = (animComponent.currentFrame + 1) % animComponent.frames.size();
                auto &[ image, transform, color ] = animComponent.frames.at(animComponent.currentFrame);
                spriteComponent.setImage(image);
                spriteComponent.setColor(color);
//            spriteComponent.setTransform(transform);
            }
        }


    }
}
