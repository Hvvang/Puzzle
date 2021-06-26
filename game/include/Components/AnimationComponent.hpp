#pragma once

#include <EntityManager.hpp>
#include <Components/Sprite.hpp>
#include <tuple>

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
    void addFrame(const ::std::string &frameName, const Transformable::Transform &transform, const Color &color) {
        frames.emplace_back(::std::make_tuple(frameName, transform, color));
    }

    enum State {
        Start,
        Play,
        Stop
    } state{Stop};

    float speed = 1.f;
    bool isLoop = false;

    ::std::chrono::milliseconds animTime{ 0 };
    ::std::chrono::milliseconds frameDuration{ 0 };

private:
    ::std::chrono::milliseconds startTime{ 0 };
    ::std::chrono::milliseconds endTime{ 0 };
    ::std::vector<::std::tuple<::std::string, Transformable::Transform, Color>> frames;
    ::std::vector<Transformable::Transform> framesTransforms;
    ::std::vector<Color> framesColor;
    uint8_t currentFrame = {0};

friend class AnimationSystem;
};