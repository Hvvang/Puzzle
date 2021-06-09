#pragma once


#include <EntityComponentSystem.hpp>
#include <Components/BoardComponent.hpp>


using ::Engine::ECS::System;
using ::Engine::ECS::Requires;

class PieceController;
class TileController;

class BoardController : public System<Requires<BoardComponent>> {
public:

    BoardController(MiniKit::Engine::Context &context);

    void update(float deltaTime);

private:

    void occupyPos(const Vector2i &pos, TileComponent *target);

    void deOccupyPos(const Vector2i &pos);

    void moveTileDown(const Vector2i &pos);

    void removeTilesInRow(int row);

    void checkLinesToClear();
    void clearLines();

private:
    ::std::shared_ptr<PieceController> m_pieceSystem{ nullptr };
    ::std::shared_ptr<TileController> m_tileSystem{ nullptr };
    ::std::unique_ptr<Entity> m_currentPiece{ nullptr };

    ::std::vector<int> m_linesToClear;
    Vector2i m_spawnLocation{4, 0};
};
