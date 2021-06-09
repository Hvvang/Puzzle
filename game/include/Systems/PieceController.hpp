#pragma once

#include <EntityComponentSystem.hpp>
#include <Components/PieceComponent.hpp>

using ::Engine::ECS::System;
using ::Engine::ECS::Requires;

class TileController;
class PieceComponent;

class PieceController : public System<Requires<PieceComponent>>, public ::MiniKit::Platform::Responder {
public:
    PieceController(::std::shared_ptr<TileController> tileController) : tileSystem(tileController) {};
    void update(float deltaTime);

    void spawnPiece(PieceComponent &piece, PieceComponent::Shape shape, const Vector2i &spawnLocation);

    ::std::array<Vector2i, 4> getTilesPosition(const PieceComponent &piece);
    void movePiece(PieceComponent &piece, const Vector2i &offset);
    void rotatePiece(PieceComponent &piece, bool clockwise);

    void KeyDown(::MiniKit::Platform::Window &window, const ::MiniKit::Platform::KeyEvent &event) noexcept override;
    void KeyUp(::MiniKit::Platform::Window &window, const ::MiniKit::Platform::KeyEvent &event) noexcept override;

private:
    float localTime = 0.f;
    float moveTime = 0.f;

    std::shared_ptr<TileController> tileSystem{ nullptr };
};


