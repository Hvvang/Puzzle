#pragma once

#include "Font/Font.hpp"

#include <EntityComponentSystem.hpp>
#include <Math.hpp>

#include <memory>

#include <Event/Event.hpp>
#include <Event/EventSystem.hpp>

#include <GameEvents.hpp>

class GridController;
class CollisionSystem;
class PieceController;
class TileController;
class ScoreSystem;

class TextRenderer;

namespace Engine::ECS {
    class Entity;
    class SpriteRenderingSystem;
}

namespace MiniKit::Engine {
    class Context;
}

using ::Engine::Math::Vector2i;
using ::Engine::ECS::Entity;
using ::Engine::ECS::SpriteRenderingSystem;

class GameController {
public:
    GameController(MiniKit::Engine::Context &context);
    ~GameController();

    void update(float deltaTime);

private:
    void initPlayFieldBackground();
    void updatePieces(BlockSetEvent *event = nullptr);
    void spawnPiece();
    void updateNextPiece();

private:
    MiniKit::Engine::Context &m_context;

    ::std::unique_ptr<EventSystem> m_eventSystem{ nullptr };
    ::std::unique_ptr<Entity> m_playField{ nullptr };

    Entity m_currentPiece;
    ::std::unique_ptr<Entity> m_nextPiece{ nullptr };

    ::std::unique_ptr<Entity> m_levelUp{ nullptr };

    ::std::shared_ptr<SpriteRenderingSystem> m_renderingSystem{ nullptr };
    ::std::shared_ptr<TextRenderer> m_textRenderer{ nullptr };
    ::std::shared_ptr<GridController> m_gridSystem{nullptr };
    ::std::shared_ptr<CollisionSystem> m_collisionSystem{ nullptr };
    ::std::shared_ptr<PieceController> m_pieceSystem{ nullptr };
    ::std::shared_ptr<TileController> m_tileSystem{ nullptr };
    ::std::shared_ptr<ScoreSystem> m_scoreSystem{ nullptr };

    friend class GridController;
    friend class CollisionSystem;
    friend class PieceController;
    friend class TileController;
    friend class ScoreSystem;


};
