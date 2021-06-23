#pragma once

#include <EntityComponentSystem.hpp>
#include <Components/BoardComponent.hpp>
#include <Event/GameEvents.hpp>

using ::Engine::ECS::System;
using ::Engine::ECS::Requires;

class GameController;

class GridController : public System<Requires<BoardComponent>> {
public:
    explicit GridController(GameController *parent = nullptr);

    void update(float deltaTime);

    bool isInBounds(const Vector2i &pos);

    bool isPosEmpty(const Vector2i &pos);

    void resetBoard();

    void activate();
    void deactivate();

private:

    void occupyPos(const Vector2i &pos, TileComponent *target);

    void deOccupyPos(const Vector2i &pos);

    void moveTileDown(const Vector2i &pos);

    void removeTilesInRow(int row);

    void checkLinesToClear();

    void clearLines();

    void onBlockSetEvent(BlockSetEvent *blockSetEvent);

private:
    GameController *m_parent{ nullptr };
    ::std::vector<int> m_linesToClear;
    Vector2i m_spawnLocation{4, 0};

    friend class GameController;
};
