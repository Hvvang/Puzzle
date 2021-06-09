#include <Systems/BoardController.hpp>

void BoardController::occupyPos(const Vector2i &pos, TileComponent *target) {
    auto entities = getEntities();
    for (auto &entity : entities) {
        auto &board = entity.getComponent<BoardComponent>();

        board.m_grid[pos.x][pos.y]->isOccupied = true;
        board.m_grid[pos.x][pos.y]->tile = target;
    }

}

void BoardController::deOccupyPos(const Vector2i &pos) {
    auto entities = getEntities();
    for (auto &entity : entities) {
        auto &board = entity.getComponent<BoardComponent>();

        board.m_grid[pos.x][pos.y]->isOccupied = false;
        board.m_grid[pos.x][pos.y]->tile = nullptr;
    }
}

void BoardController::moveTileDown(const Vector2i &pos) {
    auto entities = getEntities();
    for (auto &entity : entities) {
        auto &board = entity.getComponent<BoardComponent>();

        if (board.m_grid[pos.x][pos.y]->isOccupied) {
            occupyPos({pos.x, pos.y + 1}, board.m_grid[pos.x][pos.y]->tile);
            deOccupyPos(pos);
        }
    }
}

void BoardController::removeTilesInRow(int row) {
    for (auto col = 0; col < COLS; ++col) {
        deOccupyPos({col, row});
        moveTileDown({col, row - 1});
    }
}

void BoardController::update(float deltaTime) {

}

void BoardController::checkLinesToClear() {
    auto entities = getEntities();
    for (auto &entity : entities) {
        auto &board = entity.getComponent<BoardComponent>();

        for (int row = 0; row < ROWS; ++row) {
            bool toggle = true;

            for (int col = 0; col < COLS; ++col) {
                if (!board.m_grid[col][row]->isOccupied) {
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

void BoardController::clearLines() {
    for (auto &lineIndex : m_linesToClear) {
        removeTilesInRow(lineIndex);
    }
    m_linesToClear.clear();
}
