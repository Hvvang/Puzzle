#include <Systems/TileController.hpp>

using ::Engine::Math::operator+=;
using ::Engine::Math::operator-;
using ::Engine::Math::IntRect;

bool TileController::canMove(const Vector2i &pos) {
    // TODO: Fixme - Implement me
    return false;
}

void TileController::move(const Vector2i &offset) {
    auto entities = getEntities();
    for (auto &entity : entities) {
        auto &tile = entity.getComponent<TileComponent>();
        tile.position += offset;
        tile.updateSpritePosition();
    }
}

void TileController::updatePosition(const Vector2i &pos) {
    auto entities = getEntities();
    for (auto &entity : entities) {
        auto &tile = entity.getComponent<TileComponent>();

        tile.position = pos;
        tile.updateSpritePosition();
    }
}

void TileController::rotate(const Vector2i &origin, bool clockwise) {
    auto entities = getEntities();
    for (auto &entity : entities) {
        auto &tile = entity.getComponent<TileComponent>();

        if (tile.active) {
            Vector2i relativePos = tile.position - origin;
            IntRect rotMatrix = clockwise
                                ? IntRect{ Vector2i{0, -1}, Vector2i{1, 0} }
                                : IntRect{ Vector2i{0, 1}, Vector2i{-1, 0} };
            int newXPos = (rotMatrix.x * relativePos.x) + (rotMatrix.width * relativePos.y);
            int newYPos = (rotMatrix.y * relativePos.x) + (rotMatrix.height * relativePos.y);
            Vector2i newPos = {newXPos, newYPos};

            newPos += origin;
            updatePosition(newPos);
        }
    }


}


