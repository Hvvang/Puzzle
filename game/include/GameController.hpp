#pragma once


#include <EntityComponentSystem.hpp>
#include "Font/Font.hpp"
#include <Math.hpp>

#include <Event/Event.hpp>
#include <Event/EventSystem.hpp>
#include <Event/GameEvents.hpp>

#include <memory>

class App;

class GridController;
class CollisionSystem;
class PieceController;
class TileController;
class ScoreSystem;

namespace Engine::ECS {
    class Entity;

}

namespace MiniKit::Engine {
    class Context;
}

using ::Engine::Math::Vector2i;
using ::Engine::ECS::Entity;

class GameController {
public:
    explicit GameController(App *parent, MiniKit::Engine::Context &context);
    ~GameController();

    void update(float deltaTime);

    void initPlayFieldBackground();
    void updatePieces(BlockSetEvent *event = nullptr);

    void pauseGame();
    void overGame();
    void resetGame();
    void resumeGame();
    void activate();
    void deactivate();

private:
    void spawnPiece();
    void updateNextPiece();

private:
    App *m_parent{ nullptr };
    MiniKit::Engine::Context &m_context;

    ::std::unique_ptr<Entity> m_playField{ nullptr };
    ::std::unique_ptr<Entity> m_currentPiece{ nullptr };
    ::std::unique_ptr<Entity> m_nextPiece{ nullptr };
    ::std::unique_ptr<Entity> m_ghostPiece{ nullptr };

    ::std::unique_ptr<EventSystem> m_eventSystem{ nullptr };
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
