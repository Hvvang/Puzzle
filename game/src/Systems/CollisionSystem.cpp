#include "Systems/CollisionSystem.hpp"
#include "Systems/PieceController.hpp"
#include "Systems/BoardController.hpp"

#include <Components/PieceCollisionComponent.hpp>
#include <Components/PieceComponent.hpp>

using ::Engine::Math::operator-;


void CollisionSystem::update(float) {
    auto colliders = getEntities();
    for (auto &collider : colliders) {
        auto &piece = collider.getComponent<PieceComponent>();
        auto &collision = collider.getComponent<PieceCollisionComponent>();

        if (piece.state & PieceComponent::State::RotateRight
            || piece.state & PieceComponent::State::RotateLeft) {
            Vector2i endOffset{0, 0};
            bool clockwise = piece.state & PieceComponent::State::RotateRight;

            if (!checkOffset(piece, collision, piece.previousData.rotationIndex, piece.rotationIndex, endOffset)) {
                m_pieceSystem->rotatePiece(piece, !clockwise);
            } else {
                m_pieceSystem->movePiece(piece, endOffset);
            }
        }
        else if (!checkMovePiece(m_pieceSystem->getTilesPosition(piece), {0, 0})) {
            if (piece.state & PieceComponent::State::MoveRight
                || piece.state & PieceComponent::State::MoveLeft) {
                bool isRightMove = (piece.state & PieceComponent::State::MoveRight);
                bool isLeftMove = (piece.state & PieceComponent::State::MoveLeft);
                auto backOffset = Vector2i{-isRightMove + isLeftMove, 0};

                m_pieceSystem->movePiece(piece, backOffset);
            }
            else if (piece.state & PieceComponent::State::MoveDown
                || piece.state & PieceComponent::State::SoftDownMove) {
                m_pieceSystem->movePiece(piece, {0, -1});
            }
        }
    }
}

bool CollisionSystem::checkOffset(const PieceComponent &piece, const PieceCollisionComponent &collision,
                                  int oldRotIndex, int newRotIndex, Vector2i &endOffset) {
    Vector2i offsetVal1, offsetVal2;

    for (int testIndex = 0; testIndex < 5; testIndex++) {
        offsetVal1 = collision.offsetMatrix[testIndex][oldRotIndex];
        offsetVal2 = collision.offsetMatrix[testIndex][newRotIndex];
        endOffset = offsetVal1 - offsetVal2;

        if (checkMovePiece(m_pieceSystem->getTilesPosition(piece), endOffset)) {
            return true;
        }
    }
    return false;
}

bool CollisionSystem::checkMovePiece(const ::std::array<Vector2i, 4> &positions, const Vector2i &offset) {
    for (auto &pos : positions) {
        auto resPos = pos + offset;

        if (!m_boardSystem->isInBounds(resPos) || !m_boardSystem->isPosEmpty(resPos)) {
            return false;
        }
    }
    return true;
}
