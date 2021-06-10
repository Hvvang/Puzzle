#pragma once

#include <EntityComponentSystem.hpp>

#include <Math.hpp>

using ::Engine::ECS::System;
using ::Engine::ECS::Requires;
using ::Engine::Math::Vector2i;
using ::Engine::Math::Vector2f;

class GameController;
class TileComponent;

class TileController : public System<Requires<TileComponent>> {
public:
    explicit TileController(GameController *parent = nullptr) : m_parent(parent) {};

    void update(float deltaTime);

    void move(TileComponent &tile, const Vector2i &offset);
    void updatePosition(TileComponent &tile, const Vector2i &pos, const Vector2f &boardOffset = {5.f, 4.f});
    void rotate(TileComponent &tile, const Vector2i &origin, bool clockwise);

private:
    GameController *m_parent = { nullptr };

    friend class PieceController;
};