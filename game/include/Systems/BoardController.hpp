#pragma once


#include <EntityComponentSystem.hpp>
#include <Components/BoardComponent.hpp>


using ::Engine::ECS::System;
using ::Engine::ECS::Requires;

class BoardController : public System<Requires<BoardComponent>> {
public:

    void update(float deltaTime);

private:

    void occupyPos(const Vector2i &pos, TileComponent *target);

    void deOccupyPos(const Vector2i &pos);

    void moveTileDown(const Vector2i &pos);

    void removeTilesInRow(int row);

    void checkLinesToClear();
    void clearLines();
private:
    ::std::vector<int> m_linesToClear;

};
