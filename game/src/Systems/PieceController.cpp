#include "Systems/PieceController.hpp"

#include <Systems/TileController.hpp>

using ::Engine::Math::operator-;

void PieceController::update(float deltaTime) {
    localTime += deltaTime;
    moveTime += deltaTime;

    auto entities = getEntities();
    for (auto &entity : entities) {
        auto &pieceComponent = entity.getComponent<PieceComponent>();
        pieceComponent.previousData = {pieceComponent.state, pieceComponent.rotationIndex};

        if (pieceComponent.state & PieceComponent::State::MoveLeft) {
            if (moveTime > 0.2f) {
                movePiece(pieceComponent, {-1, 0});
                moveTime = 0;
            }
        }
        if (pieceComponent.state & PieceComponent::State::MoveRight) {
            if (moveTime > 0.2f) {
                movePiece(pieceComponent, {1, 0});
                moveTime = 0;
            }
        }
        if (pieceComponent.state & PieceComponent::State::SoftDownMove) {
            if (moveTime > 0.2f) {
                movePiece(pieceComponent, {0, 1});
                moveTime = 0;
            }
        }
        if (pieceComponent.state & PieceComponent::State::RotateLeft) {
            if (moveTime > 0.2f) {
                rotatePiece(pieceComponent, false);
                moveTime = 0.f;
            }
        }
        if (pieceComponent.state & PieceComponent::State::RotateRight) {
            if (moveTime > 0.2f) {
                rotatePiece(pieceComponent, true);
                moveTime = 0.f;
            }
        }
//        if (localTime >= pieceComponent.m_speed) {
//            movePiece(pieceComponent, {0, 1});
//            localTime = 0.f;
//        }
    }
}

void PieceController::spawnPiece(PieceComponent &piece, PieceComponent::Shape shape, const Vector2i &spawnLocation) {
    piece.shape = shape;
    tileSystem->updatePosition(piece.tiles[0]->getComponent<TileComponent>(), spawnLocation);
    switch (piece.shape) {
        case PieceComponent::Shape::I:
            tileSystem->updatePosition(piece.tiles[1]->getComponent<TileComponent>(),spawnLocation + Vector2i{-1, 0});
            tileSystem->updatePosition(piece.tiles[2]->getComponent<TileComponent>(),spawnLocation + Vector2i{2, 0});
            tileSystem->updatePosition(piece.tiles[3]->getComponent<TileComponent>(),spawnLocation + Vector2i{1, 0});
            break;
        case PieceComponent::Shape::O:
            tileSystem->updatePosition(piece.tiles[1]->getComponent<TileComponent>(),spawnLocation + Vector2i{1, 0});
            tileSystem->updatePosition(piece.tiles[2]->getComponent<TileComponent>(),spawnLocation + Vector2i{1, -1});
            tileSystem->updatePosition(piece.tiles[3]->getComponent<TileComponent>(),spawnLocation + Vector2i{0, -1});
            break;
        case PieceComponent::Shape::J:
            tileSystem->updatePosition(piece.tiles[1]->getComponent<TileComponent>(),spawnLocation + Vector2i{-1, 0});
            tileSystem->updatePosition(piece.tiles[2]->getComponent<TileComponent>(),spawnLocation + Vector2i{-1, -1});
            tileSystem->updatePosition(piece.tiles[3]->getComponent<TileComponent>(),spawnLocation + Vector2i{1, 0});
            break;
        case PieceComponent::Shape::L:
            tileSystem->updatePosition(piece.tiles[1]->getComponent<TileComponent>(),spawnLocation + Vector2i{-1, 0});
            tileSystem->updatePosition(piece.tiles[2]->getComponent<TileComponent>(),spawnLocation + Vector2i{1, -1});
            tileSystem->updatePosition(piece.tiles[3]->getComponent<TileComponent>(),spawnLocation + Vector2i{1, 0});
            break;
        case PieceComponent::Shape::T:
            tileSystem->updatePosition(piece.tiles[1]->getComponent<TileComponent>(),spawnLocation + Vector2i{-1, 0});
            tileSystem->updatePosition(piece.tiles[2]->getComponent<TileComponent>(),spawnLocation + Vector2i{0, -1});
            tileSystem->updatePosition(piece.tiles[3]->getComponent<TileComponent>(),spawnLocation + Vector2i{1, 0});
            break;
        case PieceComponent::Shape::Z:
            tileSystem->updatePosition(piece.tiles[1]->getComponent<TileComponent>(),spawnLocation + Vector2i{0, -1});
            tileSystem->updatePosition(piece.tiles[2]->getComponent<TileComponent>(),spawnLocation + Vector2i{-1, -1});
            tileSystem->updatePosition(piece.tiles[3]->getComponent<TileComponent>(),spawnLocation + Vector2i{1, 0});
            break;
        case PieceComponent::Shape::S:
            tileSystem->updatePosition(piece.tiles[1]->getComponent<TileComponent>(),spawnLocation + Vector2i{-1, 0});
            tileSystem->updatePosition(piece.tiles[2]->getComponent<TileComponent>(),spawnLocation + Vector2i{1, -1});
            tileSystem->updatePosition(piece.tiles[3]->getComponent<TileComponent>(),spawnLocation + Vector2i{0, -1});
            break;
    }
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
        tileSystem->move(piece.tiles[i]->getComponent<TileComponent>(), offset);
    }
}

int Mod(int x, int m) {
    return (x % m + m) % m;
}

void PieceController::rotatePiece(PieceComponent &piece, bool clockwise) {
    piece.rotationIndex += clockwise ? 1 : -1;
    piece.rotationIndex = Mod(piece.rotationIndex, 4);

    auto originPos = piece.tiles[0]->getComponent<TileComponent>().position;
    for(int i = 0; i < 4; ++i) {
        tileSystem->rotate(piece.tiles[i]->getComponent<TileComponent>(), originPos, clockwise);
    }
}

void PieceController::KeyDown(MiniKit::Platform::Window &window, const MiniKit::Platform::KeyEvent &event) noexcept {
    auto entities = getEntities();
    for (auto &entity : entities) {
        auto &pieceComponent = entity.getComponent<PieceComponent>();

        uint8_t state = static_cast<uint8_t>(pieceComponent.state);

        if (event.keycode == MiniKit::Platform::Keycode::KeyLeft) {
            state |= static_cast<uint8_t>(PieceComponent::State::MoveLeft);
        }
        if (event.keycode == MiniKit::Platform::Keycode::KeyRight) {
            state |= static_cast<uint8_t>(PieceComponent::State::MoveRight);
        }
        if (event.keycode == MiniKit::Platform::Keycode::KeyDown) {
            state |= static_cast<uint8_t>(PieceComponent::State::SoftDownMove);
        }
        if (event.keycode == MiniKit::Platform::Keycode::KeySpace) {
            state |= static_cast<uint8_t>(PieceComponent::State::HardDownMove);
        }
        if (event.keycode == MiniKit::Platform::Keycode::KeyX) {
            state |= static_cast<uint8_t>(PieceComponent::State::RotateRight);
        }
        if (event.keycode == MiniKit::Platform::Keycode::KeyZ) {
            state |= static_cast<uint8_t>(PieceComponent::State::RotateLeft);
        }
        pieceComponent.state = static_cast<PieceComponent::State>(state);
    }
}

void PieceController::KeyUp(MiniKit::Platform::Window &window, const MiniKit::Platform::KeyEvent &event) noexcept {
    auto entities = getEntities();
    for (auto &entity : entities) {
        auto &pieceComponent = entity.getComponent<PieceComponent>();
        uint8_t state = static_cast<uint8_t>(pieceComponent.state);

        if (event.keycode == MiniKit::Platform::Keycode::KeyLeft) {
            state &= ~static_cast<uint8_t>(PieceComponent::State::MoveLeft);
        }
        if (event.keycode == MiniKit::Platform::Keycode::KeyRight) {
            state &= ~static_cast<uint8_t>(PieceComponent::State::MoveRight);
        }
        if (event.keycode == MiniKit::Platform::Keycode::KeyDown) {
            state &= ~static_cast<uint8_t>(PieceComponent::State::SoftDownMove);
        }
        if (event.keycode == MiniKit::Platform::Keycode::KeySpace) {
            state &= ~static_cast<uint8_t>(PieceComponent::State::HardDownMove);
        }
        if (event.keycode == MiniKit::Platform::Keycode::KeyX) {
            state &= ~static_cast<uint8_t>(PieceComponent::State::RotateRight);
        }
        if (event.keycode == MiniKit::Platform::Keycode::KeyZ) {
            state &= ~static_cast<uint8_t>(PieceComponent::State::RotateLeft);
        }
        pieceComponent.state = static_cast<PieceComponent::State>(state);

    }


}




