#pragma once

#include <EntityManager.hpp>
#include <Components/Sprite.hpp>

namespace Engine::ECS {
    class Component;
    class Entity;
    class Sprite;
    class Transformable;
}

using ::Engine::ECS::Transformable;
using ::Engine::ECS::Component;
using ::Engine::ECS::Entity;
using ::Engine::ECS::Sprite;

struct AnimationComponent : Component {

    AnimationComponent() {
        instance = ::std::make_unique<Entity>(entityManager->createEntity());
        instance->addComponent<Sprite>();
    }

    void addFrame(const ::std::string &frameName) {
        frames.emplace_back(frameName);
    }

    void setCurrentFrame(uint8_t index) {
        currentFrame = index;
        instance->getComponent<Sprite>().setImage(frames.at(index));
    }

    Transformable::Transform &getTransform() {
        return instance->getComponent<Sprite>().getTransform();
    }

    Color &getColor() {
        return instance->getComponent<Sprite>().getColor();
    }

    enum State {
        Start,
        Play,
        Stop
    } state{Start};

    float speed = 1.f;
    bool isLoop = false;

    ::std::chrono::milliseconds animTime{ 0 };
    ::std::chrono::milliseconds frameDuration{ 0 };

private:
    ::std::chrono::milliseconds startTime{ 0 };
    ::std::chrono::milliseconds endTime{ 0 };
    ::std::vector<::std::string> frames;
    uint8_t currentFrame = {0};

    ::std::unique_ptr<Entity> instance{ nullptr };

friend class AnimationSystem;
};