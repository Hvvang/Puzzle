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
class AnimationSystem;

namespace Engine::ECS {
    class Entity;
}

namespace MiniKit::Engine {
    class Context;
}
using ::MiniKit::Platform::Responder;
using ::MiniKit::Platform::Window;
using ::MiniKit::Platform::KeyEvent;

using ::Engine::Math::Vector2i;
using ::Engine::ECS::Entity;

class GameController : public Responder  {
    enum class State {
        Spawn,
        Fall,
        ClearLine,
        PieceBlocking,
        GameOver,
        Pause,
        Off
    } m_currentState{State::Off};

public:
    explicit GameController(App *parent, MiniKit::Engine::Context &context);
    ~GameController();

    void update(float deltaTime);

    void initPlayFieldBackground();
    void updatePieces(SpawnPieceEvent *event = nullptr);

    void activate();
    void deactivate();

    void NewGameState();
    void ResumeGameState();

    bool hasGame() { return m_currentState != State::Off; }

    uint8_t getHardDropDistance();


private:
    void spawnPiece();
    void updateNextPiece();
    void updateGhostPiece();

    void onPieceFallen(PieceFallenEvent *);
    void onGameOver(GameOverEvent *);
    void onLevelUp(LevelUpEvent *);

    void KeyDown(Window &window, const KeyEvent &event) noexcept override;

private:
    App *m_parent{ nullptr };
    MiniKit::Engine::Context &m_context;

    ::std::chrono::milliseconds now;
    ::std::chrono::milliseconds inputDelay{0};
    ::std::chrono::milliseconds m_pieceBlockingTimer{0};

    ::std::unique_ptr<Entity> m_levelUpAnim{ nullptr };
    ::std::unique_ptr<Entity> m_pauseLabel{ nullptr };
    ::std::unique_ptr<Entity> m_gameOverLabel{ nullptr };
    ::std::unique_ptr<Entity> m_controlInfoLabel{ nullptr };
    ::std::unique_ptr<Entity> m_playField{ nullptr };
    ::std::unique_ptr<Entity> m_currentPiece{ nullptr };
    ::std::unique_ptr<Entity> m_nextPiece{ nullptr };
    ::std::unique_ptr<Entity> m_ghostPiece{ nullptr };

    ::std::unique_ptr<EventSystem> m_eventSystem{ nullptr };
    ::std::shared_ptr<GridController> m_gridSystem{ nullptr };
    ::std::shared_ptr<CollisionSystem> m_collisionSystem{ nullptr };
    ::std::shared_ptr<PieceController> m_pieceSystem{ nullptr };
    ::std::shared_ptr<TileController> m_tileSystem{ nullptr };
    ::std::shared_ptr<ScoreSystem> m_scoreSystem{ nullptr };
    ::std::shared_ptr<AnimationSystem> m_animationSystem{ nullptr };


    friend class GridController;
    friend class CollisionSystem;
    friend class PieceController;
    friend class TileController;
    friend class ScoreSystem;
};
