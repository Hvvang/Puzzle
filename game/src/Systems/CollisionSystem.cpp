#include "Systems/CollisionSystem.hpp"
#include <Systems/PieceController.hpp>
#include <Systems/GridController.hpp>

#include <Components/PieceCollisionComponent.hpp>

#include <GameController.hpp>

#include <Event/GameEvents.hpp>

#include <Components/MoveComponent.hpp>

using ::Engine::Math::operator-;

void CollisionSystem::update(float) {
    auto colliders = getEntities();
    for (auto &collider : colliders) {
        auto &piece = collider.getComponent<PieceComponent>();
        auto &moveComponent = collider.getComponent<MoveComponent>();
        auto &collision = collider.getComponent<PieceCollisionComponent>();

        if (moveComponent.state & (MoveComponent::State::RotateRight | MoveComponent::State::RotateLeft)) {
            Vector2i endOffset{0, 0};
            bool clockwise = moveComponent.state & MoveComponent::State::RotateRight;

            if (!checkOffset(piece, collision, moveComponent.previousData.rotationIndex,
                             moveComponent.rotationIndex, endOffset)) {
                m_parent->m_pieceSystem->rotatePiece(piece, moveComponent, !clockwise);
            } else {
                m_parent->m_pieceSystem->movePiece(piece, endOffset);
            }
        }
        else if (!checkMovePiece(m_parent->m_pieceSystem->getTilesPosition(piece), {0, 0})) {
            if (moveComponent.state & (MoveComponent::State::MoveRight | MoveComponent::State::MoveLeft)) {
                bool isRightMove = (moveComponent.state & MoveComponent::State::MoveRight);
                bool isLeftMove = (moveComponent.state & MoveComponent::State::MoveLeft);
                auto backOffset = Vector2i{-isRightMove + isLeftMove, 0};

                m_parent->m_pieceSystem->movePiece(piece, backOffset);
            } else {
                m_parent->m_pieceSystem->movePiece(piece, {0, -1});
                m_parent->m_eventSystem->emit(new PieceFallenEvent());
            }
        }
//        if (!checkMovePiece(m_parent->m_pieceSystem->getTilesPosition(piece), {0, 1})) {
//            m_parent->m_eventSystem->emit(new GameOverEvent());
//        }
    }

}

bool CollisionSystem::checkOffset(const PieceComponent &piece, const PieceCollisionComponent &collision,
                                  int oldRotIndex, int newRotIndex, Vector2i &endOffset) {
    Vector2i offsetVal1, offsetVal2;

    for (int testIndex = 0; testIndex < 5; testIndex++) {
        offsetVal1 = collision.offsetMatrix[testIndex][oldRotIndex];
        offsetVal2 = collision.offsetMatrix[testIndex][newRotIndex];
        endOffset = offsetVal1 - offsetVal2;

        if (checkMovePiece(m_parent->m_pieceSystem->getTilesPosition(piece), endOffset)) {
            return true;
        }
    }
    return false;
}

bool CollisionSystem::checkMovePiece(const ::std::array<Vector2i, 4> &positions, const Vector2i &offset) {

    for (auto &pos : positions) {
        auto resPos = pos + offset;

        if (!m_parent->m_gridSystem->isInBounds(resPos) || !m_parent->m_gridSystem->isPosEmpty(resPos)) {
            return false;
        }
    }
    return true;
}
