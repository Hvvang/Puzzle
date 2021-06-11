#include "Systems/PieceController.hpp"

#include <Systems/TileController.hpp>

#include <GameController.hpp>

#include <Components/MoveComponent.hpp>

using ::Engine::Math::operator-;

void PieceController::update(float deltaTime) {
    localTime += deltaTime;
    moveTime += deltaTime;

    auto entities = getEntities();
    for (auto &entity : entities) {
        auto &pieceComponent = entity.getComponent<PieceComponent>();
        auto &moveComponent = entity.getComponent<MoveComponent>();

        moveComponent.previousData = {moveComponent.state, moveComponent.rotationIndex};

        if (moveComponent.state & MoveComponent::State::MoveLeft) {
            if (moveTime > 0.2f) {
                movePiece(pieceComponent, {-1, 0});
                moveTime = 0;
            }
        }
        if (moveComponent.state & MoveComponent::State::MoveRight) {
            if (moveTime > 0.2f) {
                movePiece(pieceComponent, {1, 0});
                moveTime = 0;
            }
        }
        if (moveComponent.state & MoveComponent::State::SoftDownMove) {
            if (moveTime > 0.2f) {
                movePiece(pieceComponent, {0, 1});
                moveTime = 0;
            }
        }
        if (moveComponent.state & MoveComponent::State::RotateLeft) {
            if (moveTime > 0.2f) {
                rotatePiece(pieceComponent, moveComponent, false);
                moveTime = 0.f;
            }
        }
        if (moveComponent.state & MoveComponent::State::RotateRight) {
            if (moveTime > 0.2f) {
                rotatePiece(pieceComponent, moveComponent, true);
                moveTime = 0.f;
            }
        }
        if (localTime >= moveComponent.m_speed) {
            movePiece(pieceComponent, {0, 1});
            localTime = 0.f;
        }
    }
}

void PieceController::setColor(PieceComponent &piece, PieceComponent::Shape shape) {
    Color color;
    switch (shape) {
        case PieceComponent::Shape::I:
            color = {0.f, 0.5f, 0.7f, 1.f};
            break;
        case PieceComponent::Shape::O:
            color = {0.7f, 0.5f, 0.f, 1.f};
            break;
        case PieceComponent::Shape::J:
            color = {0.6f, 0.2f, 0.1f, 1.f};
            break;
        case PieceComponent::Shape::L:
            color = {0.7f, 0.1f, 0.f, 1.f};
            break;
        case PieceComponent::Shape::T:
            color = {0.7f, 0.8f, 0.f, 1.f};
            break;
        case PieceComponent::Shape::Z:
            color = {0.f, 0.5f, 0.1f, 1.f};
            break;
        case PieceComponent::Shape::S:
            color = {0.9f, 0.f, 0.8f, 1.f};
            break;
    }
    for (auto i = 0u; i < 4; ++i) {
        m_parent->m_tileSystem->updateColor(piece.tiles[i]->getComponent<TileComponent>(), color);
    }
}

void PieceController::spawnPiece(PieceComponent &piece, PieceComponent::Shape shape, const Vector2i &spawnLocation, const Vector2f &boardOffset) {
    auto &tileSystem = m_parent->m_tileSystem;
    piece.shape = shape;
    tileSystem->updatePosition(piece.tiles[0]->getComponent<TileComponent>(), spawnLocation, boardOffset);
    switch (piece.shape) {
        case PieceComponent::Shape::I:
            tileSystem->updatePosition(piece.tiles[1]->getComponent<TileComponent>(),spawnLocation + Vector2i{-1, 0}, boardOffset);
            tileSystem->updatePosition(piece.tiles[2]->getComponent<TileComponent>(),spawnLocation + Vector2i{2, 0}, boardOffset);
            tileSystem->updatePosition(piece.tiles[3]->getComponent<TileComponent>(),spawnLocation + Vector2i{1, 0}, boardOffset);
            break;
        case PieceComponent::Shape::O:
            tileSystem->updatePosition(piece.tiles[1]->getComponent<TileComponent>(),spawnLocation + Vector2i{1, 0}, boardOffset);
            tileSystem->updatePosition(piece.tiles[2]->getComponent<TileComponent>(),spawnLocation + Vector2i{1, -1}, boardOffset);
            tileSystem->updatePosition(piece.tiles[3]->getComponent<TileComponent>(),spawnLocation + Vector2i{0, -1}, boardOffset);
            break;
        case PieceComponent::Shape::J:
            tileSystem->updatePosition(piece.tiles[1]->getComponent<TileComponent>(),spawnLocation + Vector2i{-1, 0}, boardOffset);
            tileSystem->updatePosition(piece.tiles[2]->getComponent<TileComponent>(),spawnLocation + Vector2i{-1, -1}, boardOffset);
            tileSystem->updatePosition(piece.tiles[3]->getComponent<TileComponent>(),spawnLocation + Vector2i{1, 0}, boardOffset);
            break;
        case PieceComponent::Shape::L:
            tileSystem->updatePosition(piece.tiles[1]->getComponent<TileComponent>(),spawnLocation + Vector2i{-1, 0}, boardOffset);
            tileSystem->updatePosition(piece.tiles[2]->getComponent<TileComponent>(),spawnLocation + Vector2i{1, -1}, boardOffset);
            tileSystem->updatePosition(piece.tiles[3]->getComponent<TileComponent>(),spawnLocation + Vector2i{1, 0}, boardOffset);
            break;
        case PieceComponent::Shape::T:
            tileSystem->updatePosition(piece.tiles[1]->getComponent<TileComponent>(),spawnLocation + Vector2i{-1, 0}, boardOffset);
            tileSystem->updatePosition(piece.tiles[2]->getComponent<TileComponent>(),spawnLocation + Vector2i{0, -1}, boardOffset);
            tileSystem->updatePosition(piece.tiles[3]->getComponent<TileComponent>(),spawnLocation + Vector2i{1, 0}, boardOffset);
            break;
        case PieceComponent::Shape::Z:
            tileSystem->updatePosition(piece.tiles[1]->getComponent<TileComponent>(),spawnLocation + Vector2i{0, -1}, boardOffset);
            tileSystem->updatePosition(piece.tiles[2]->getComponent<TileComponent>(),spawnLocation + Vector2i{-1, -1}, boardOffset);
            tileSystem->updatePosition(piece.tiles[3]->getComponent<TileComponent>(),spawnLocation + Vector2i{1, 0}, boardOffset);
            break;
        case PieceComponent::Shape::S:
            tileSystem->updatePosition(piece.tiles[1]->getComponent<TileComponent>(),spawnLocation + Vector2i{-1, 0}, boardOffset);
            tileSystem->updatePosition(piece.tiles[2]->getComponent<TileComponent>(),spawnLocation + Vector2i{1, -1}, boardOffset);
            tileSystem->updatePosition(piece.tiles[3]->getComponent<TileComponent>(),spawnLocation + Vector2i{0, -1}, boardOffset);
            break;
    }
    setColor(piece, shape);
}

::std::array<Vector2i, 4> PieceController::getTilesPosition(const PieceComponent &piece) {
    ::std::array<Vector2i, 4> positions{};
    for (auto i = 0; i < 4; ++i) {
        positions[i] = piece.tiles[i]->getComponent<TileComponent>().position;
    }
    return positions;
}

void PieceController::movePiece(PieceComponent &piece, const Vector2i &offset) {
    for (auto i = 0u; i < 4; ++i) {
        m_parent->m_tileSystem->move(piece.tiles[i]->getComponent<TileComponent>(), offset);
    }
}

int Mod(int x, int m) {
    return (x % m + m) % m;
}

void PieceController::rotatePiece(PieceComponent &piece, MoveComponent &move, bool clockwise) {

    move.rotationIndex += clockwise ? 1 : -1;
    move.rotationIndex = Mod(move.rotationIndex, 4);

    auto originPos = piece.tiles[0]->getComponent<TileComponent>().position;
    for(int i = 0; i < 4; ++i) {
        m_parent->m_tileSystem->rotate(piece.tiles[i]->getComponent<TileComponent>(), originPos, clockwise);
    }
}

void PieceController::KeyDown(MiniKit::Platform::Window &window, const MiniKit::Platform::KeyEvent &event) noexcept {
    auto entities = getEntities();
    for (auto &entity : entities) {
        auto &moveComponent = entity.getComponent<MoveComponent>();

        uint8_t state = static_cast<uint8_t>(moveComponent.state);

        if (event.keycode == MiniKit::Platform::Keycode::KeyLeft) {
            state |= static_cast<uint8_t>(MoveComponent::State::MoveLeft);
        }
        if (event.keycode == MiniKit::Platform::Keycode::KeyRight) {
            state |= static_cast<uint8_t>(MoveComponent::State::MoveRight);
        }
        if (event.keycode == MiniKit::Platform::Keycode::KeyDown) {
            state |= static_cast<uint8_t>(MoveComponent::State::SoftDownMove);
        }
        if (event.keycode == MiniKit::Platform::Keycode::KeySpace) {
            state |= static_cast<uint8_t>(MoveComponent::State::HardDownMove);
        }
        if (event.keycode == MiniKit::Platform::Keycode::KeyX) {
            state |= static_cast<uint8_t>(MoveComponent::State::RotateRight);
        }
        if (event.keycode == MiniKit::Platform::Keycode::KeyZ) {
            state |= static_cast<uint8_t>(MoveComponent::State::RotateLeft);
        }
        moveComponent.state = static_cast<MoveComponent::State>(state);
    }
}

void PieceController::KeyUp(MiniKit::Platform::Window &window, const MiniKit::Platform::KeyEvent &event) noexcept {
    auto entities = getEntities();
    for (auto &entity : entities) {
        auto &moveComponent = entity.getComponent<MoveComponent>();
        uint8_t state = static_cast<uint8_t>(moveComponent.state);

        if (event.keycode == MiniKit::Platform::Keycode::KeyLeft) {
            state &= ~static_cast<uint8_t>(MoveComponent::State::MoveLeft);
        }
        if (event.keycode == MiniKit::Platform::Keycode::KeyRight) {
            state &= ~static_cast<uint8_t>(MoveComponent::State::MoveRight);
        }
        if (event.keycode == MiniKit::Platform::Keycode::KeyDown) {
            state &= ~static_cast<uint8_t>(MoveComponent::State::SoftDownMove);
        }
        if (event.keycode == MiniKit::Platform::Keycode::KeySpace) {
            state &= ~static_cast<uint8_t>(MoveComponent::State::HardDownMove);
        }
        if (event.keycode == MiniKit::Platform::Keycode::KeyX) {
            state &= ~static_cast<uint8_t>(MoveComponent::State::RotateRight);
        }
        if (event.keycode == MiniKit::Platform::Keycode::KeyZ) {
            state &= ~static_cast<uint8_t>(MoveComponent::State::RotateLeft);
        }
        moveComponent.state = static_cast<MoveComponent::State>(state);

    }


}




