#include <Systems/GridController.hpp>

#include <Components/PieceComponent.hpp>
#include <Components/PieceCollisionComponent.hpp>

#include <Systems/TileController.hpp>
#include <Systems/PieceController.hpp>
#include <Systems/CollisionSystem.hpp>

#include <GameController.hpp>

GridController::GridController(GameController *parent)
    : m_parent(parent) {
    m_parent->m_eventSystem->connect(this, &GridController::onBlockSetEvent);
}

void GridController::occupyPos(const Vector2i &pos, TileComponent *target) {
    auto entities = getEntities();
    for (auto &entity : entities) {
        auto &board = entity.getComponent<BoardComponent>();

        board.m_grid.at(pos.y).at(pos.x)->isOccupied = true;
        board.m_grid.at(pos.y).at(pos.x)->tile = target;
    }
}

void GridController::deOccupyPos(const Vector2i &pos) {
    if (pos.y >= 0) {
        auto entities = getEntities();
        for (auto &entity : entities) {
            auto &board = entity.getComponent<BoardComponent>();

            board.m_grid.at(pos.y).at(pos.x)->isOccupied = false;
            board.m_grid.at(pos.y).at(pos.x)->tile = nullptr;
        }
    }
}

void GridController::moveTileDown(const Vector2i &pos) {
    auto entities = getEntities();
    for (auto &entity : entities) {
        auto &board = entity.getComponent<BoardComponent>();

        if (board.m_grid[pos.y][pos.x]->isOccupied) {
            occupyPos({pos.x, pos.y + 1}, board.m_grid[pos.y][pos.x]->tile);
            m_parent->m_tileSystem->updatePosition(*board.m_grid[pos.y][pos.x]->tile, {pos.x, pos.y + 1});
            deOccupyPos(pos);
        }
    }
}

void GridController::removeTilesInRow(int row) {
    auto entities = getEntities();
    for (auto &entity : entities) {
        auto &board = entity.getComponent<BoardComponent>();

        for (auto col = 0; col < COLS; ++col) {
            board.m_grid[row][col]->tile->instance->erase();
            deOccupyPos({col, row});
        }
        for (auto r = row - 1; r >= 0; --r) {
            for (auto col = 0; col < COLS; ++col) {
                moveTileDown({col, r});
            }
        }
    }
}

void GridController::update(float deltaTime) {

}

void GridController::checkLinesToClear() {
    auto entities = getEntities();
    for (auto &entity : entities) {
        auto &board = entity.getComponent<BoardComponent>();

        for (int row = 0; row < ROWS; ++row) {
            bool toggle = true;

            for (int col = 0; col < COLS; ++col) {
                if (!board.m_grid[row][col]->isOccupied) {
                    toggle = false;
                    break;
                }
            }
            if (toggle) {
                m_linesToClear.push_back(row);
            }
        }
    }
}

void GridController::clearLines() {
    for (auto &lineIndex : m_linesToClear) {
        removeTilesInRow(lineIndex);
    }
    m_parent->m_eventSystem->emit(new LinesClearEvent(m_linesToClear));
    m_linesToClear.clear();
}

bool GridController::isInBounds(const Vector2i &pos) {
    return pos.x >= 0 && pos.x < COLS && pos.y < ROWS;
}

bool GridController::isPosEmpty(const Vector2i &pos) {
    if (pos.y >= 0) {
        auto entities = getEntities();
        for (auto &entity : entities) {
            auto &board = entity.getComponent<BoardComponent>();
            return !board.m_grid[pos.y][pos.x]->isOccupied;
        }
    }
    return true;
}

void GridController::onBlockSetEvent(BlockSetEvent *) {
    auto &piece = m_parent->m_currentPiece.getComponent<PieceComponent>();

    for (auto &tile : piece.tiles) {
        auto &component = tile->getComponent<TileComponent>();
        occupyPos(component.position, &component);
    }
    checkLinesToClear();
    clearLines();
}
