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
        I = 73, O = 79, J = 74, L = 76, T = 84, Z = 90, S = 83
    } shape;

    PieceComponent() {
        for (auto &tile : tiles) {
            tile = ::std::make_unique<Entity>(entityManager->createEntity());
            tile->addComponent<TileComponent>();
        }
    }

    void activate() {
        for (auto &tile : tiles) {
            tile->activate();
            tile->getComponent<TileComponent>().instance->activate();
        }
    }

    void deactivate() {
        for (auto &tile : tiles) {
            tile->deactivate();
            tile->getComponent<TileComponent>().instance->deactivate();
        }
    }

private:
    ::std::array<::std::unique_ptr<Entity>, 4> tiles{ nullptr };
    Color color {0.1, 0.1, 0.1};

    friend class PieceController;
    friend class CollisionSystem;
    friend class GridController;
};