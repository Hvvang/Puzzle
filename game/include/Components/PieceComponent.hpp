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

#define TileX 42.7f
#define TileY 34.f
#define OffsetX 6.f
#define OffsetY 6.f

class PieceComponent : public Component {
public:

    enum class Shape : uint8_t {
        IShape,
        OShape,
        JShape,
        LShape,
        TShape,
        ZShape,
        SShape
    } shape;

    explicit PieceComponent(Shape shape, Color color = {1.f, 1.f, 0.f, 1.f}) {

        m_shape = getShape(shape);

        auto tileIndex = 0u;
        for (auto i = 0; i < 4; ++i) {
            for (auto j = 0; j < 4; ++j) {
                if (m_shape[i][j]) {
                    auto pos = Vector2i{j, i};
                    auto &block = m_blocks.at(tileIndex);

                    block = ::std::make_unique<Entity>(entityManager->createEntity());
                    block->addComponent<TileComponent>();
                    block->activate();

                    tileIndex++;
                }
            }
        }
    }

    void move(Vector2i offset) {
        position += offset;
        for (auto &block : m_blocks) {
            auto &component = block->getComponent<TileComponent>();
            component.updateSpritePosition();
        }
    }

    Vector2f gridToScreenPos(Vector2i pos) {
        return {3.f + pos.x * (TileX + OffsetX), 3.f + pos.y * (TileY + OffsetY)};
    }

   float getSpeed() const {
        return m_speed;
    }


private:
    auto getShape(Shape shape) -> ::std::array<::std::array<bool, 4>, 4> {
        switch (shape) {
            case Shape::IShape:
                return {
                        {
                                {0, 0, 1, 0},
                                {0, 0, 1, 0},
                                {0, 0, 1, 0},
                                {0, 0, 1, 0}
                        }
                };
            case Shape::OShape:
                return {
                        {
                                {0, 1, 1, 0},
                                {0, 1, 1, 0},
                                {0, 0, 0, 0},
                                {0, 0, 0, 0}
                        }
                };
            case Shape::JShape:
                return {
                        {
                                {0, 0, 1, 0},
                                {0, 0, 1, 0},
                                {0, 1, 1, 0},
                                {0, 0, 0, 0}
                        }
                };
            case Shape::LShape:
                return {
                        {
                                {0, 1, 0, 0},
                                {0, 1, 0, 0},
                                {0, 1, 1, 0},
                                {0, 0, 0, 0}
                        }
                };
            case Shape::TShape:
                return {
                        {
                                {0, 0, 1, 0},
                                {0, 1, 1, 0},
                                {0, 0, 1, 0},
                                {0, 0, 0, 0}
                        }
                };
            case Shape::ZShape:
                return {
                        {
                                {1, 1, 0, 0},
                                {0, 1, 1, 0},
                                {0, 0, 0, 0},
                                {0, 0, 0, 0}
                        }
                };
            case Shape::SShape:
                return {
                        {
                                {0, 1, 1, 0},
                                {1, 1, 0, 0},
                                {0, 0, 0, 0},
                                {0, 0, 0, 0}
                        }
                };
        }
    }


private:
    ::std::array<::std::unique_ptr<Entity>, 4> m_blocks{ nullptr };
    ::std::array<::std::array<bool, 4>, 4> m_shape;

    /// Position on grid
    Vector2i position{3, 0};

    float m_speed = 0.6f;
};