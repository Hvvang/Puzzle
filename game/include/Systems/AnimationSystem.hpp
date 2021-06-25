#pragma once

#include <EntityComponentSystem.hpp>
#include <Math.hpp>
#include <utility>

#include <Components/AnimationComponent.hpp>

using ::Engine::ECS::System;
using ::Engine::ECS::Requires;
using ::Engine::Math::Vector2i;

class AnimationSystem : public System<Requires<AnimationComponent>> {
public:
    void update(float deltaTime);
};

