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

struct PieceComponent : Component {

    enum class Shape : uint8_t {
        I, O, J, L, T, Z, S
    } shape;

    PieceComponent() {
        for (auto &tile : tiles) {
            tile = ::std::make_unique<Entity>(entityManager->createEntity());
            tile->addComponent<TileComponent>();
        }
    }

private:
    ::std::array<::std::unique_ptr<Entity>, 4> tiles{ nullptr };

    friend class PieceController;
    friend class CollisionSystem;
    friend class GridController;
};