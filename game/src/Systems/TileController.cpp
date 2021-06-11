#include <Systems/TileController.hpp>

#include <Components/TileComponent.hpp>

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

void TileController::updatePosition(TileComponent &tile, const Vector2i &pos, const Vector2f &boardOffset) {
    tile.position = pos;
    auto &spriteComponent = tile.instance->getComponent<Sprite>();
    spriteComponent.getTransform().position = {boardOffset.x + (pos.x) * (TileX + OffsetX),
                                               boardOffset.y + (pos.y) * (TileY + OffsetY)};
}

void TileController::updateColor(TileComponent &tile, const Color &color) {
    tile.color = color;
    auto &spriteComponent = tile.instance->getComponent<Sprite>();
    spriteComponent.getColor() = color;
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






