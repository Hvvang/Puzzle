#pragma once

#include <EntityComponentSystem.hpp>
#include <Math.hpp>
#include <utility>

using ::Engine::ECS::System;
using ::Engine::ECS::Requires;
using ::Engine::Math::Vector2i;

class PieceController;
class BoardController;
class PieceCollisionComponent;
class PieceComponent;

class CollisionSystem : public System<Requires<PieceComponent, PieceCollisionComponent>> {
public:
    explicit CollisionSystem(BoardController *board, ::std::shared_ptr<PieceController> pieceController)
        : m_boardSystem(board)
        , m_pieceSystem(pieceController) {}

    void update(float deltaTime);

private:

    bool checkMovePiece(const ::std::array<Vector2i, 4> &positions, const Vector2i &offset);

    bool checkOffset(const PieceComponent &piece, const PieceCollisionComponent &collision,
                     int oldRotIndex, int newRotIndex, Vector2i &endOffset);

private:
    ::std::shared_ptr<PieceController> m_pieceSystem{ nullptr };
    BoardController *m_boardSystem{ nullptr };

};
