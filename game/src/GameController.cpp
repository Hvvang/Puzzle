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

    SpriteManager->loadSprite("assets/images/background_b.png", "playField");
    SpriteManager->loadSprite("assets/images/square.png", "square");
    SpriteManager->loadSprite("assets/images/pause.png", "pause");
    SpriteManager->loadSprite("assets/images/info_b.png", "info");

    entityManager->addSystem(*m_gridSystem);
    entityManager->addSystem(*m_collisionSystem);
    entityManager->addSystem(*m_pieceSystem);
    entityManager->addSystem(*m_tileSystem);
    entityManager->addSystem(*m_scoreSystem);

    m_controlInfoLabel = ::std::make_unique<Entity>(entityManager->createEntity());
    m_pauseLabel = ::std::make_unique<Entity>(entityManager->createEntity());
    m_ghostPiece = ::std::make_unique<Entity>(entityManager->createEntity());
    m_currentPiece = ::std::make_unique<Entity>(entityManager->createEntity());
    m_nextPiece = ::std::make_unique<Entity>(entityManager->createEntity());

    m_playField = ::std::make_unique<Entity>(entityManager->createEntity());

    m_pauseLabel->addComponent<Sprite>();
    m_controlInfoLabel->addComponent<Sprite>();

    m_playField->addComponent<BoardComponent>();
    m_playField->addComponent<ScoreComponent>();
    m_playField->addComponent<Sprite>();

    m_ghostPiece->addComponent<PieceComponent>();
    m_nextPiece->addComponent<PieceComponent>();

    initPlayFieldBackground();

    m_eventSystem->connect(this, &GameController::updatePieces);
    m_eventSystem->connect(this, &GameController::onPieceFallen);
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
                m_parent->ChangeState();
                break;
        }
        inputDelay = now;
    }
}

void GameController::initPlayFieldBackground() {
    auto &playFieldSprite = m_playField->getComponent<Sprite>();

    playFieldSprite.setImage("playField");
    playFieldSprite.getTransform().scale *= 2.f;

    auto &pauseLabelSprite = m_pauseLabel->getComponent<Sprite>();
    pauseLabelSprite.setImage("pause");
    pauseLabelSprite.getTransform().position.y += m_context.GetWindow().GetHeight() / 5;
    pauseLabelSprite.getTransform().scale *= 2.f;

    auto &infoLabelSprite = m_controlInfoLabel->getComponent<Sprite>();
    infoLabelSprite.setImage("info");
    infoLabelSprite.getTransform().position = {m_context.GetWindow().GetWidth() / 4.f, m_context.GetWindow().GetHeight() / 4.f};
    infoLabelSprite.getTransform().scale *= 2.5f;
}

void GameController::updatePieces(BlockSetEvent *) {
    if (m_currentPiece->isActivated()) {
        m_currentPiece->erase();
    }
    spawnPiece();
    updateNextPiece();
}

uint8_t translateIndex(int index) {
    switch (index) {
        case 0:
            return 73;
        case 1:
            return 79;
        case 2:
            return 74;
        case 3:
            return 76;
        case 4:
            return 84;
        case 5:
            return 90;
        case 6:
            return 83;
    }
}

void GameController::updateNextPiece() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distribution(0, 6);

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

    m_pieceSystem->spawnPiece(pieceComponent, shape, m_gridSystem->m_spawnLocation);
    m_currentPiece->activate();
}

void GameController::update(float deltaTime) {
    if (m_currentState != State::Pause) {
        m_pieceSystem->update(deltaTime);
        m_collisionSystem->update(deltaTime);
        m_gridSystem->update(deltaTime);
        m_scoreSystem->update(deltaTime);
    }
}


void GameController::activate() {
    m_currentState = State::Fall;
    m_context.GetWindow().AddResponder(*this);
    m_context.GetWindow().AddResponder(*m_pieceSystem);

    m_playField->activate();

    m_nextPiece->activate();

    if (settings->getValue("Ghost piece")) {
        m_ghostPiece->activate();
        m_ghostPiece->getComponent<PieceComponent>().activate();
    }

    entityManager->refresh();
    m_gridSystem->activate();
    m_scoreSystem->activate();
}

void GameController::deactivate() {
    m_context.GetWindow().RemoveResponder(*this);
    m_context.GetWindow().RemoveResponder(*m_pieceSystem);

    m_playField->deactivate();
    m_gridSystem->deactivate();
    m_scoreSystem->deactivate();
    m_currentPiece->deactivate();
    m_nextPiece->deactivate();
    m_ghostPiece->deactivate();

    m_playField->getComponent<ScoreComponent>().deactivate();
    m_currentPiece->getComponent<PieceComponent>().deactivate();
    m_nextPiece->getComponent<PieceComponent>().deactivate();
    m_ghostPiece->getComponent<PieceComponent>().deactivate();

    entityManager->refresh();
}


void GameController::NewGameState() {
    activate();
    m_gridSystem->resetBoard();
    m_scoreSystem->resetScore();
    updateNextPiece();
    updatePieces();
}

void GameController::ResumeGameState() {
    activate();

    m_currentPiece->activate();
    m_currentPiece->getComponent<PieceComponent>().activate();
    m_nextPiece->getComponent<PieceComponent>().activate();
}




