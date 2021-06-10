#pragma once

#include <EntityComponentSystem.hpp>
#include <Math.hpp>

#include <array>

#include <Components/TileComponent.hpp>

class GridController;

using ::Engine::ECS::Component;
using ::Engine::Math::FloatRect;

#define ROWS 20
#define COLS 10

struct BoardBlock {
    const Vector2i location = {0, 0};
    TileComponent *tile = nullptr;
    bool isOccupied = false;

    explicit BoardBlock(const Vector2i &pos) : location(pos) {}
};

class BoardComponent : public Component {
public:
    BoardComponent() {
        for (auto i = 0; i < ROWS; ++i) {
            for (auto j = 0; j < COLS; ++j) {
                m_grid[i][j] = ::std::make_unique<BoardBlock>(Vector2i{i, j});
            }
        }
    }


private:
    ::std::array<::std::array<::std::unique_ptr<BoardBlock>, COLS>, ROWS> m_grid{ nullptr };

    friend class GridController;
};

