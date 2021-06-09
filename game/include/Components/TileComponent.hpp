#pragma once

#include <memory>

#include <EntityManager.hpp>
#include <Components/Sprite.hpp>

using ::Engine::ECS::Entity;
using ::Engine::ECS::Component;
using ::Engine::ECS::Sprite;
using ::Engine::Math::Vector2i;

#define TileX 42.7f
#define TileY 34.f
#define OffsetX 6.f
#define OffsetY 6.f

struct TileComponent : Component {
    TileComponent() {
        entity = ::std::make_unique<Entity>(entityManager->createEntity());

        auto &spriteComponent = entity->addComponent<Sprite>();
        spriteComponent.setImage("square");
        spriteComponent.getColor() = color;
        spriteComponent.getTransform().scale = {TileX * 2.f / static_cast<float>(spriteComponent.getTileRect().size.x),
                                                TileY * 2.f / static_cast<float>(spriteComponent.getTileRect().size.y)};
        entity->activate();
    }

    void updateSpritePosition() {
        auto &spriteComponent = entity->getComponent<Sprite>();
        spriteComponent.getTransform().position = {3.f + (position.x) * (TileX + OffsetX),
                                                   4.f + (position.y) * (TileY + OffsetY)};
    }

    Vector2i position;
    Color color{0.f, 0.5f, 1.f, 1.f};

    bool active = true;

private:
    ::std::unique_ptr<Entity> entity{ nullptr };
};