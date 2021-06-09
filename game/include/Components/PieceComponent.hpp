#pragma once

#include <EntityManager.hpp>
#include <Components/Sprite.hpp>
#include <Components/TileComponent.hpp>

#include <array>
#include <memory>

class TileComponent;

using ::Engine::ECS::Entity;
using ::Engine::ECS::Component;
using ::Engine::ECS::Sprite;
using ::Engine::Math::Vector2i;
using ::Engine::Math::operator+=;
using ::Engine::Math::operator+;

//#define TileX 43.7f
//#define TileY 34.f
//#define OffsetX 6.f
//#define OffsetY 6.f

struct PieceComponent : Component {


    enum class Shape : uint8_t {
        I, O, J, L, T, Z, S
    } shape;

    enum State : uint8_t {
        MoveLeft = 0x1,
        MoveRight = 0x2,
        MoveDown = 0x4,
        SoftDownMove = 0x8,
        HardDownMove = 0x10,
        RotateLeft = 0x20,
        RotateRight = 0x40,
    } state;

    struct Data {
        State state;
        int rotationIndex;
    } previousData;

    PieceComponent() : state(State::MoveDown) {
        for (auto &tile : tiles) {
            tile = ::std::make_unique<Entity>(entityManager->createEntity());
            tile->addComponent<TileComponent>();
        }
    }

private:
    ::std::array<::std::unique_ptr<Entity>, 4> tiles{ nullptr };

    int rotationIndex{0};
    float m_speed = 0.6f;

    friend class PieceController;
    friend class CollisionSystem;
};