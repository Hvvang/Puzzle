#pragma once

#include <EntityComponentSystem.hpp>
#include <Components/PieceComponent.hpp>

using ::Engine::ECS::System;
using ::Engine::ECS::Requires;

class GameController;
class PieceComponent;

class PieceController : public System<Requires<PieceComponent>>, public ::MiniKit::Platform::Responder {
public:
    explicit PieceController(GameController *parent = nullptr) : m_parent(parent) {};

    void update(float deltaTime);

    void spawnPiece(PieceComponent &piece, PieceComponent::Shape shape, const Vector2i &spawnLocation);

    ::std::array<Vector2i, 4> getTilesPosition(const PieceComponent &piece);
    void movePiece(PieceComponent &piece, const Vector2i &offset);
    void rotatePiece(PieceComponent &piece, bool clockwise);

    void KeyDown(::MiniKit::Platform::Window &window, const ::MiniKit::Platform::KeyEvent &event) noexcept override;
    void KeyUp(::MiniKit::Platform::Window &window, const ::MiniKit::Platform::KeyEvent &event) noexcept override;

private:
    GameController *m_parent = { nullptr };

    float localTime = 0.f;
    float moveTime = 0.f;

};


