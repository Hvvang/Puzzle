#include <Systems/TileController.hpp>

using ::Engine::Math::operator+=;
using ::Engine::Math::operator-;
using ::Engine::Math::operator+;
using ::Engine::Math::IntRect;


void TileController::update(float deltaTime) {
    auto entities = getEntities();
    for (auto &entity : entities) {
        auto &tile = entity.getComponent<TileComponent>();

    }
}

void TileController::move(TileComponent &tile, const Vector2i &offset) {
    updatePosition(tile, tile.position + offset);
}

void TileController::updatePosition(TileComponent &tile, const Vector2i &pos) {
    tile.position = pos;
    auto &spriteComponent = tile.instatnce->getComponent<Sprite>();
    spriteComponent.getTransform().position = {5.f + (pos.x) * (TileX + OffsetX),
                                               4.f + (pos.y) * (TileY + OffsetY)};
}

void TileController::rotate(TileComponent &tile, const Vector2i &origin, bool clockwise) {
        Vector2i relativePos = tile.position - origin;
        IntRect rotMatrix = clockwise
                            ? IntRect{ Vector2i{0, 1}, Vector2i{-1, 0} }
                            : IntRect{ Vector2i{0, -1}, Vector2i{1, 0} };
        int newXPos = (rotMatrix.x * relativePos.x) + (rotMatrix.width * relativePos.y);
        int newYPos = (rotMatrix.y * relativePos.x) + (rotMatrix.height * relativePos.y);
        Vector2i newPos = {newXPos, newYPos};

        newPos += origin;
        updatePosition(tile, newPos);
}




