#pragma once

#include <EntityComponentSystem.hpp>
#include <Components/TileComponent.hpp>

using ::Engine::ECS::System;
using ::Engine::ECS::Requires;

class TileController : public System<Requires<TileComponent>> {
public:
    bool canMove(const Vector2i &pos);
    void move(const Vector2i &offset);
    void updatePosition(const Vector2i &pos);
    void rotate(const Vector2i &origin, bool clockwise);
};