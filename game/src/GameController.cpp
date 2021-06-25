#include "GameController.hpp"

#include <MiniKit/MiniKit.hpp>


#include <Systems/GridController.hpp>
#include <Systems/CollisionSystem.hpp>
#include <Systems/PieceController.hpp>
#include <Systems/TileController.hpp>
#include <Systems/ScoreSystem.hpp>

#include <random>
#include <App.hpp>
#include <Settings.hpp>

using ::Engine::Math::operator*=;

GameController::GameController(App *parent, MiniKit::Engine::Context &context) : m_parent(parent), m_context(context) {
    m_eventSystem = ::std::make_unique<EventSystem>();

    m_gridSystem = ::std::make_shared<GridController>(this);
    m_collisionSystem = ::std::make_shared<CollisionSystem>(this);
    m_pieceSystem = ::std::make_shared<PieceController>(this);
    m_tileSystem = ::std::make_shared<TileController>(this);
    m_scoreSystem = ::std::make_shared<ScoreSystem>(this);

    SpriteManager->loadSprite("assets/images/background_b.png", "playField_0");
    SpriteManager->loadSprite("assets/images/background_g.png", "playField_1");
    SpriteManager->loadSprite("assets/images/background_p.png", "playField_2");
    SpriteManager->loadSprite("assets/images/info_b.png", "info_0");
    SpriteManager->loadSprite("assets/images/info_g.png", "info_1");
    SpriteManager->loadSprite("assets/images/info_p.png", "info_2");
    SpriteManager->loadSprite("assets/images/square.png", "square");
    SpriteManager->loadSprite("assets/images/pause.png", "pause");
    SpriteManager->loadSprite("assets/images/gameOver.png", "gameOver");

    entityManager->addSystem(*m_gridSystem);
    entityManager->addSystem(*m_collisionSystem);
    entityManager->addSystem(*m_pieceSystem);
    entityManager->addSystem(*m_tileSystem);
    entityManager->addSystem(*m_scoreSystem);

    m_controlInfoLabel = ::std::make_unique<Entity>(entityManager->createEntity());
    m_pauseLabel = ::std::make_unique<Entity>(entityManager->createEntity());
    m_gameOverLabel = ::std::make_unique<Entity>(entityManager->createEntity());
    m_ghostPiece = ::std::make_unique<Entity>(entityManager->createEntity());
    m_currentPiece = ::std::make_unique<Entity>(entityManager->createEntity());
    m_nextPiece = ::std::make_unique<Entity>(entityManager->createEntity());

    m_playField = ::std::make_unique<Entity>(entityManager->createEntity());

    m_pauseLabel->addComponent<Sprite>().getTransform().scale *= 2.f;
    m_gameOverLabel->addComponent<Sprite>().getTransform().scale *= 1.8f;
    m_controlInfoLabel->addComponent<Sprite>().getTransform().scale *= 2.5f;

    m_playField->addComponent<BoardComponent>();
    m_playField->addComponent<ScoreComponent>();
    m_playField->addComponent<Sprite>().getTransform().scale *= 2.f;

    m_ghostPiece->addComponent<PieceComponent>();
    m_nextPiece->addComponent<PieceComponent>();


    m_eventSystem->connect(this, &GameController::updatePieces);
    m_eventSystem->connect(this, &GameController::onPieceFallen);
    m_eventSystem->connect(this, &GameController::onGameOver);
}

GameController::~GameController() {
//    m_context.GetWindow().RemoveResponder(*m_pieceSystem);
//    m_context.GetWindow().RemoveResponder(*this);
}

void GameController::onPieceFallen(PieceFallenEvent *) {
    if (m_currentState != State::PieceBlocking) {
        m_currentState = State::PieceBlocking;
        m_pieceBlockingTimer = ::std::chrono::duration_cast<::std::chrono::milliseconds>(::std::chrono::system_clock::now().time_since_epoch());
    } else {
        auto currentTime = ::std::chrono::duration_cast<::std::chrono::milliseconds>(::std::chrono::system_clock::now().time_since_epoch());
        if (currentTime - m_pieceBlockingTimer > std::chrono::milliseconds(500)) {
            m_currentState = State::Fall;
            m_eventSystem->emit(new BlockSetEvent);
        }
    }
}



void GameController::KeyDown(Window &window, const KeyEvent &event) noexcept {
    now = ::std::chrono::duration_cast<::std::chrono::milliseconds>(::std::chrono::system_clock::now().time_since_epoch());

    if (now - inputDelay > std::chrono::milliseconds(130)) {
        switch (event.keycode) {
            case MiniKit::Platform::Keycode::KeyP:
                if (m_currentState != State::Pause) {
                    m_currentState = State::Pause;
                    m_pauseLabel->activate();
                }
                else {
                    m_pauseLabel->deactivate();
                    m_currentState = State::Fall;
                }
                break;
            case MiniKit::Platform::Keycode::KeyI:
                if (m_controlInfoLabel->isActivated()) {
                    m_controlInfoLabel->deactivate();
                    m_pauseLabel->activate();
                } else {
                    m_currentState = State::Pause;
                    m_controlInfoLabel->activate();
                }
                break;
            case MiniKit::Platform::Keycode::KeyQ:
                if (m_currentState == State::GameOver) {
                    m_currentState = State::Off;
                    m_gameOverLabel->deactivate();
                }
                m_parent->ChangeState();
                break;
        }
        inputDelay = now;
    }
}

void GameController::initPlayFieldBackground() {
    auto &playFieldSprite = m_playField->getComponent<Sprite>();

    playFieldSprite.setImage("playField_" + ::std::to_string(int(settings->getValue("Theme"))));

    auto &pauseLabelSprite = m_pauseLabel->getComponent<Sprite>();
    pauseLabelSprite.setImage("pause");
    pauseLabelSprite.getTransform().position.y = m_context.GetWindow().GetHeight() / 5;

    auto &gameOverLabelSprite = m_gameOverLabel->getComponent<Sprite>();
    gameOverLabelSprite.setImage("gameOver");
    gameOverLabelSprite.getTransform().position.x = m_context.GetWindow().GetWidth() / 40;
    gameOverLabelSprite.getTransform().position.y = m_context.GetWindow().GetHeight() / 3;

    auto &infoLabelSprite = m_controlInfoLabel->getComponent<Sprite>();
    infoLabelSprite.setImage("info_" + ::std::to_string(int(settings->getValue("Theme"))));
    infoLabelSprite.getTransform().position = {m_context.GetWindow().GetWidth() / 4.f, m_context.GetWindow().GetHeight() / 4.f};
}

void GameController::updatePieces(SpawnPieceEvent *) {
    if (m_currentPiece->isActivated()) {
        m_currentPiece->erase();
    }
    spawnPiece();
    updateNextPiece();
}

uint8_t translateIndex(float frequency) {
    if (settings->getValue("Tetrominos")["T"].at(2)["Frequency"] >= frequency) {
        return 'T';
    }
    if (settings->getValue("Tetrominos")["L"].at(2)["Frequency"] >= frequency) {
        return 'L';
    }
    if (settings->getValue("Tetrominos")["J"].at(2)["Frequency"] >= frequency) {
        return 'J';
    }
    if (settings->getValue("Tetrominos")["Z"].at(2)["Frequency"] >= frequency) {
        return 'Z';
    }
    if (settings->getValue("Tetrominos")["S"].at(2)["Frequency"] >= frequency) {
        return 'S';
    }
    if (settings->getValue("Tetrominos")["O"].at(2)["Frequency"] >= frequency) {
        return 'O';
    }
    if (settings->getValue("Tetrominos")["I"].at(2)["Frequency"] >= frequency) {
        return 'I';
    }
}

void GameController::updateNextPiece() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> distribution(0.05f, 0.7f);

    auto shape = static_cast<PieceComponent::Shape>(translateIndex(distribution(gen)));

    auto &pieceComponent = m_nextPiece->getComponent<PieceComponent>();
    m_pieceSystem->spawnPiece(pieceComponent, shape, {11, 6}, {16.f, 4.f});
    pieceComponent.activate();
}

void GameController::updateGhostPiece() {
    auto &pieceComponent = m_ghostPiece->getComponent<PieceComponent>();
    bool isPossible = true;
    auto row = 0;
    auto currentPiecePos = m_pieceSystem->getTilesPosition(m_currentPiece->getComponent<PieceComponent>());

    for (; row < 20 && isPossible; ++row) {
        for (auto pos : currentPiecePos) {
            pos.y += row;
            if (!m_gridSystem->isInBounds(pos) || !m_gridSystem->isPosEmpty(pos)) {
                --row;
                isPossible = false;
                break;
            }
        }
    }
    for (auto &pos : currentPiecePos) {
        pos.y += (row - 1);
    }
    m_pieceSystem->spawnGhost(pieceComponent, currentPiecePos);
}

uint8_t GameController::getHardDropDistance() {
    auto &currentPiece = m_currentPiece->getComponent<PieceComponent>();
    auto &ghostPiece = m_ghostPiece->getComponent<PieceComponent>();
    return ghostPiece.tiles[0]->getComponent<TileComponent>().position.y - currentPiece.tiles[0]->getComponent<TileComponent>().position.y;
}

void GameController::spawnPiece() {
    m_currentPiece = ::std::make_unique<Entity>(entityManager->createEntity());
    
    auto &pieceComponent = m_currentPiece->addComponent<PieceComponent>();
    auto shape = m_nextPiece->getComponent<PieceComponent>().shape;
    pieceComponent.activate();

    m_currentPiece->addComponent<PieceCollisionComponent>(shape);
    m_currentPiece->addComponent<MoveComponent>();
    m_currentPiece->activate();

    m_pieceSystem->spawnPiece(pieceComponent, shape, m_gridSystem->m_spawnLocation);
}

void GameController::update(float deltaTime) {
    if (m_currentState != State::Pause && m_currentState != State::GameOver) {
        m_pieceSystem->update(deltaTime);
        m_collisionSystem->update(deltaTime);
        if (m_currentState == State::GameOver) {
            return;
        }
        updateGhostPiece();
        m_gridSystem->update(deltaTime);
        m_scoreSystem->update(deltaTime);
    }
}


void GameController::activate() {
    m_currentState = State::Fall;
    m_context.GetWindow().AddResponder(*this);
    m_context.GetWindow().AddResponder(*m_pieceSystem);

    m_playField->activate();
    initPlayFieldBackground();

    m_nextPiece->activate();

    entityManager->refresh();
    m_gridSystem->activate();
    m_scoreSystem->activate();
}

void GameController::deactivate() {
    m_context.GetWindow().RemoveResponder(*this);
    m_context.GetWindow().RemoveResponder(*m_pieceSystem);

    m_playField->deactivate();
    m_currentPiece->deactivate();
    m_nextPiece->deactivate();
    m_ghostPiece->deactivate();

//    m_playField->getComponent<ScoreComponent>().deactivate();
//    m_currentPiece->getComponent<PieceComponent>().deactivate();
//    m_nextPiece->getComponent<PieceComponent>().deactivate();
//    m_ghostPiece->getComponent<PieceComponent>().deactivate();

    m_gridSystem->deactivate();
    m_scoreSystem->deactivate();
    m_tileSystem->deactivate();

    entityManager->refresh();
}


void GameController::NewGameState() {
    activate();
    m_tileSystem->deleteTiles();
    m_gridSystem->resetBoard();
    m_scoreSystem->resetScore();
    m_nextPiece->addComponent<PieceComponent>();
    m_ghostPiece->addComponent<PieceComponent>();
    if (settings->getValue("Ghost piece")) {
        m_ghostPiece->activate();
        m_ghostPiece->getComponent<PieceComponent>().activate();
    }
    updateNextPiece();
    updatePieces();
}

void GameController::ResumeGameState() {
    activate();
    m_currentPiece->activate();
    m_currentPiece->getComponent<PieceComponent>().activate();
    m_nextPiece->getComponent<PieceComponent>().activate();

    if (settings->getValue("Ghost piece")) {
        m_ghostPiece->activate();
        m_ghostPiece->getComponent<PieceComponent>().activate();
    } else {
        m_ghostPiece->deactivate();
        m_ghostPiece->getComponent<PieceComponent>().deactivate();
    }
    m_tileSystem->activate();
}

void GameController::onGameOver(GameOverEvent *) {
    m_currentState = State::GameOver;
    m_gameOverLabel->activate();
}




