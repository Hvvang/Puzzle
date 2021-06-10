#pragma once

#include <memory>

#include <EntityManager.hpp>
#include <Components/Sprite.hpp>

using ::Engine::ECS::Entity;
using ::Engine::ECS::Component;
using ::Engine::ECS::Sprite;
using ::Engine::Math::Vector2i;

#define TileX 42.4f
#define TileY 33.85f
#define OffsetX 6.f
#define OffsetY 6.f

struct TileComponent : Component {
    TileComponent() {
        instatnce = ::std::make_unique<Entity>(entityManager->createEntity());

        auto &spriteComponent = instatnce->addComponent<Sprite>();
        spriteComponent.setImage("square");
        spriteComponent.getColor() = color;
        spriteComponent.getTransform().scale = {TileX * 2.f / static_cast<float>(spriteComponent.getTileRect().size.x),
                                                TileY * 2.f / static_cast<float>(spriteComponent.getTileRect().size.y)};
        instatnce->activate();
    }

private:
    bool active = true;

    ::std::unique_ptr<Entity> instatnce{ nullptr };

    Vector2i position;
    Color color{0.f, 0.f, 0.f, 1.0f};

    friend class TileController;
    friend class PieceController;
};