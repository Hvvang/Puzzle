#pragma once

#include <EntityComponentSystem.hpp>
#include <Math.hpp>
#include <utility>

#include <Components/PieceCollisionComponent.hpp>

using ::Engine::ECS::System;
using ::Engine::ECS::Requires;
using ::Engine::Math::Vector2i;

class PieceCollisionComponent;
class PieceComponent;
class GameController;

class CollisionSystem : public System<Requires<PieceComponent, PieceCollisionComponent>> {
public:
    explicit CollisionSystem(GameController *parent = nullptr) : m_parent(parent) {};

    void update(float deltaTime = 0.f);

private:

    bool checkMovePiece(const ::std::array<Vector2i, 4> &positions, const Vector2i &offset);

    bool checkOffset(const PieceComponent &piece, const PieceCollisionComponent &collision,
                     int oldRotIndex, int newRotIndex, Vector2i &endOffset);


private:
    GameController *m_parent = { nullptr };

    friend class GameController;
};
