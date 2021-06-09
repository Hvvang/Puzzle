#pragma once

#include <EntityComponentSystem.hpp>
#include <Components/TileComponent.hpp>

using ::Engine::ECS::System;
using ::Engine::ECS::Requires;

class TileController : public System<Requires<TileComponent>> {
public:
    void update(float deltaTime);

    void move(TileComponent &tile, const Vector2i &offset);
    void updatePosition(TileComponent &tile, const Vector2i &pos);
    void rotate(TileComponent &tile, const Vector2i &origin, bool clockwise);

    friend class PieceController;
};