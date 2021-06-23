#include "GameController.hpp"

#include <MiniKit/MiniKit.hpp>


#include <Systems/GridController.hpp>
#include <Systems/CollisionSystem.hpp>
#include <Systems/PieceController.hpp>
#include <Systems/TileController.hpp>
#include <Systems/ScoreSystem.hpp>

#include <random>
#include <App.hpp>


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

    entityManager->addSystem(*m_gridSystem);
    entityManager->addSystem(*m_collisionSystem);
    entityManager->addSystem(*m_pieceSystem);
    entityManager->addSystem(*m_tileSystem);
    entityManager->addSystem(*m_scoreSystem);

    m_currentPiece = ::std::make_unique<Entity>(entityManager->createEntity());
    m_nextPiece = ::std::make_unique<Entity>(entityManager->createEntity());
//    m_ghostPiece = ::std::make_unique<Entity>(entityManager->createEntity());
    m_playField = ::std::make_unique<Entity>(entityManager->createEntity());

    m_playField->addComponent<BoardComponent>();
    m_playField->addComponent<ScoreComponent>();
    m_playField->addComponent<Sprite>();

//    m_ghostPiece->addComponent<PieceComponent>();
    m_nextPiece->addComponent<PieceComponent>();

    initPlayFieldBackground();

    m_eventSystem->connect(this, &GameController::updatePieces);
}

GameController::~GameController() {
//    m_context.GetWindow().RemoveResponder(*m_pieceSystem);
//    m_context.GetWindow().RemoveResponder(*this);
}

void GameController::KeyDown(Window &window, const KeyEvent &event) noexcept {
    now = ::std::chrono::duration_cast<::std::chrono::milliseconds>(::std::chrono::system_clock::now().time_since_epoch());

    if (now - inputDelay > std::chrono::milliseconds(130)) {
        switch (event.keycode) {
            case MiniKit::Platform::Keycode::KeyP:
                if (m_currentState != State::Pause)
                    m_currentState = State::Pause;
                else
                    m_currentState = State::Fall;
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
}

void GameController::updatePieces(BlockSetEvent *) {
    if (m_currentPiece->isActivated()) {
        m_currentPiece->erase();
    }
    spawnPiece();
    updateNextPiece();
}

void GameController::updateNextPiece() {
    uint8_t shapeIndex = 0;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distribution(0, 6);
    switch (distribution(gen)) {
        case 0:
            shapeIndex = 73;
            break;
        case 1:
            shapeIndex = 79;
            break;
        case 2:
            shapeIndex = 74;
            break;
        case 3:
            shapeIndex = 76;
            break;
        case 4:
            shapeIndex = 84;
            break;
        case 5:
            shapeIndex = 90;
            break;
        case 6:
            shapeIndex = 83;
            break;
    }
    auto shape = static_cast<PieceComponent::Shape>(shapeIndex);

    auto &pieceComponent = m_nextPiece->getComponent<PieceComponent>();
    m_pieceSystem->spawnPiece(pieceComponent, shape, {11, 6}, {16.f, 4.f});
    pieceComponent.activate();
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
    m_context.GetWindow().AddResponder(*this);
    m_context.GetWindow().AddResponder(*m_pieceSystem);

    m_playField->activate();

    m_nextPiece->activate();

    m_playField->getComponent<ScoreComponent>().activate();

    entityManager->refresh();

//    m_ghostPiece->activate();
}

void GameController::deactivate() {
    m_context.GetWindow().RemoveResponder(*this);
    m_context.GetWindow().RemoveResponder(*m_pieceSystem);

    m_playField->deactivate();
    m_gridSystem->deactivate();
    m_scoreSystem->deactivate();
    m_currentPiece->deactivate();
    m_nextPiece->deactivate();

    m_playField->getComponent<ScoreComponent>().deactivate();
    m_currentPiece->getComponent<PieceComponent>().deactivate();
    m_nextPiece->getComponent<PieceComponent>().deactivate();
//    m_ghostPiece->deactivate();
    entityManager->refresh();
}


void GameController::NewGameState() {
    activate();
    m_gridSystem->resetBoard();
    m_scoreSystem->resetScore();
    m_gridSystem->activate();
    m_scoreSystem->activate();
    updateNextPiece();
    updatePieces();
}

void GameController::ResumeGameState() {
    activate();
    m_gridSystem->activate();
    m_scoreSystem->activate();
    m_currentPiece->activate();
    m_currentPiece->getComponent<PieceComponent>().activate();
    m_nextPiece->getComponent<PieceComponent>().activate();
}




