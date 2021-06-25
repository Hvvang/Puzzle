#include "Systems/PieceController.hpp"

#include <Systems/TileController.hpp>

#include <GameController.hpp>

#include <Components/MoveComponent.hpp>

#include <Settings.hpp>

using ::Engine::Math::operator-;

void PieceController::update(float deltaTime) {
    localTime += deltaTime;
    moveTime += deltaTime;

    auto entities = getEntities();
    for (auto &entity : entities) {
        auto &pieceComponent = entity.getComponent<PieceComponent>();
        auto &moveComponent = entity.getComponent<MoveComponent>();

        moveComponent.previousData.state = static_cast<MoveComponent::State>(moveComponent.state & ~MoveComponent::State::MoveDown);
        moveComponent.previousData.rotationIndex = moveComponent.rotationIndex;

        if (moveTime > 0.2 && moveComponent.state &
            (MoveComponent::State::MoveLeft | MoveComponent::State::MoveRight
            | MoveComponent::State::SoftDownMove | MoveComponent::State::HardDownMove)) {
            moveComponent.previousData.state = moveComponent.state;
            movePiece(pieceComponent, moveComponent.direction[moveComponent.state]);
            moveTime = 0;
            if (moveComponent.state & MoveComponent::State::SoftDownMove) {
                m_parent->m_eventSystem->emit(new SoftDropEvent());
            } else if (moveComponent.state & MoveComponent::State::HardDownMove) {
                auto offset = m_parent->getHardDropDistance();
                movePiece(pieceComponent, {0, offset});
                m_parent->m_eventSystem->emit(new HardDropEvent(offset));
            }
        } else if (moveTime > 0.2 && moveComponent.state & (MoveComponent::State::RotateLeft | MoveComponent::RotateRight)) {
            rotatePiece(pieceComponent, moveComponent, moveComponent.state & MoveComponent::RotateRight);
            moveTime = 0;
        }
        else if (localTime > 0.6) {
            moveComponent.previousData.state = static_cast<MoveComponent::State>(moveComponent.previousData.state |
                                                                                 MoveComponent::State::MoveDown);
            moveComponent.state = static_cast<MoveComponent::State>(MoveComponent::State::MoveDown);
            movePiece(pieceComponent, moveComponent.direction[MoveComponent::State::MoveDown]);
            localTime = 0;
        }
    }
}



void PieceController::setColor(PieceComponent &piece, const Color &color) {
    for (auto i = 0u; i < 4; ++i) {
        m_parent->m_tileSystem->updateColor(piece.tiles[i]->getComponent<TileComponent>(), color);
    }
}

void PieceController::spawnPiece(PieceComponent &piece, PieceComponent::Shape shape, const Vector2i &spawnLocation, const Vector2f &boardOffset) {
    auto &tileSystem = m_parent->m_tileSystem;
    piece.shape = shape;

    json _json_shape = settings->getValue("Tetrominos")[::std::string(1, static_cast<char>(shape))].at(0)["Shape"];
    for (unsigned i = 0; i < 4; ++i) {
        int x = _json_shape.at(i).at(0);
        int y = _json_shape.at(i).at(1);
        tileSystem->updatePosition(piece.tiles[i]->getComponent<TileComponent>(), spawnLocation + Vector2i({x, y}), boardOffset);
    }

    json _json_color = settings->getValue("Tetrominos")[::std::string(1, static_cast<char>(shape))].at(1)["Color"];
    Color color = {_json_color.at(0), _json_color.at(1), _json_color.at(2), _json_color.at(3)};

    setColor(piece, color);
}

void PieceController::spawnGhost(PieceComponent &piece, const ::std::array<Vector2i, 4> &location) {
    auto &tileSystem = m_parent->m_tileSystem;
    for (unsigned i = 0; i < 4; ++i) {
        tileSystem->updatePosition(piece.tiles[i]->getComponent<TileComponent>(), location[i], {5.f, 4.f});
    }
    json _json_color = settings->getValue("Ghost color");
    Color color = {_json_color.at(0), _json_color.at(1), _json_color.at(2), _json_color.at(3)};
    setColor(piece, color);
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

        uint8_t state = 0;

        if (event.keycode == MiniKit::Platform::Keycode::KeyLeft) {
            state |= static_cast<uint8_t>(MoveComponent::State::MoveLeft);
        }
        else if (event.keycode == MiniKit::Platform::Keycode::KeyRight) {
            state |= static_cast<uint8_t>(MoveComponent::State::MoveRight);
        }
        else if (event.keycode == MiniKit::Platform::Keycode::KeyDown) {
            state |= static_cast<uint8_t>(MoveComponent::State::SoftDownMove);
        }
        else if (event.keycode == MiniKit::Platform::Keycode::KeySpace) {
            state |= static_cast<uint8_t>(MoveComponent::State::HardDownMove);
        }
        else if (event.keycode == MiniKit::Platform::Keycode::KeyX) {
            state |= static_cast<uint8_t>(MoveComponent::State::RotateRight);
        }
        else if (event.keycode == MiniKit::Platform::Keycode::KeyZ) {
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
        else if (event.keycode == MiniKit::Platform::Keycode::KeyRight) {
            state &= ~static_cast<uint8_t>(MoveComponent::State::MoveRight);
        }
        else if (event.keycode == MiniKit::Platform::Keycode::KeyDown) {
            state &= ~static_cast<uint8_t>(MoveComponent::State::SoftDownMove);
        }
        else if (event.keycode == MiniKit::Platform::Keycode::KeySpace) {
            state &= ~static_cast<uint8_t>(MoveComponent::State::HardDownMove);
        }
        else if (event.keycode == MiniKit::Platform::Keycode::KeyX) {
            state &= ~static_cast<uint8_t>(MoveComponent::State::RotateRight);
        }
        else if (event.keycode == MiniKit::Platform::Keycode::KeyZ) {
            state &= ~static_cast<uint8_t>(MoveComponent::State::RotateLeft);
        }
        moveComponent.state = static_cast<MoveComponent::State>(state);

    }


}




