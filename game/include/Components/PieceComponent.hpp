#pragma once

#include <EntityManager.hpp>
#include <Components/Sprite.hpp>
#include <array>
#include <memory>

#include <Components/TileComponent.hpp>

using ::Engine::ECS::Entity;
using ::Engine::ECS::Component;
using ::Engine::ECS::Sprite;
using ::Engine::Math::Vector2i;
using ::Engine::Math::operator+=;
using ::Engine::Math::operator+;

#define TileX 42.7f
#define TileY 34.f
#define OffsetX 6.f
#define OffsetY 6.f

struct PieceComponent : Component {
    enum class Shape : uint8_t {
        I, O, J, L, T, Z, S
    } shape;

    enum State : uint8_t {
        MoveLeft = 0x1,
        MoveRight = 0x2,
        MoveDown = 0x4,
        SoftDownMove = 0x8,
        HardDownMove = 0x16,
        RotateLeft = 0x32,
        RotateRight = 0x64
    } state;

    PieceComponent() : state(State::MoveDown) {
        for (auto &tile : tiles) {
            tile = ::std::make_unique<Entity>(entityManager->createEntity());
            tile->addComponent<TileComponent>();
        }
    }

private:
    ::std::array<::std::unique_ptr<Entity>, 4> tiles{ nullptr };

    int rotationIndex;
    float m_speed = 0.6f;

    friend class PieceController;
};