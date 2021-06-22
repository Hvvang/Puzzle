#include "GameController.hpp"

#include <MiniKit/MiniKit.hpp>


#include <Systems/GridController.hpp>
#include <Systems/CollisionSystem.hpp>
#include <Systems/PieceController.hpp>
#include <Systems/TileController.hpp>
#include <Systems/ScoreSystem.hpp>

#include <random>



using ::Engine::Math::operator*=;

GameController::GameController(MiniKit::Engine::Context &context) : m_context(context) {
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
    m_ghostPiece = ::std::make_unique<Entity>(entityManager->createEntity());
    m_playField = ::std::make_unique<Entity>(entityManager->createEntity());

    m_playField->addComponent<BoardComponent>();
    m_playField->addComponent<ScoreComponent>();
    m_playField->addComponent<Sprite>();

    m_ghostPiece->addComponent<PieceComponent>();
    m_nextPiece->addComponent<PieceComponent>();

    initPlayFieldBackground();

    m_eventSystem->connect(this, &GameController::updatePieces);
    m_context.GetWindow().AddResponder(*m_pieceSystem);
}

GameController::~GameController() {
    m_context.GetWindow().RemoveResponder(*m_pieceSystem);
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
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distribution(0, 6);
    auto shape = static_cast<PieceComponent::Shape>(distribution(gen));

    auto &pieceComponent = m_nextPiece->getComponent<PieceComponent>();
    m_pieceSystem->spawnPiece(pieceComponent, shape, {11, 6}, {16.f, 4.f});
}


void GameController::spawnPiece() {
    m_currentPiece = ::std::make_unique<Entity>(entityManager->createEntity());
    
    auto &pieceComponent = m_currentPiece->addComponent<PieceComponent>();
    auto shape = m_nextPiece->getComponent<PieceComponent>().shape;
    m_currentPiece->addComponent<PieceCollisionComponent>(shape);
    m_currentPiece->addComponent<MoveComponent>();

    m_pieceSystem->spawnPiece(pieceComponent, shape, m_gridSystem->m_spawnLocation);
    m_currentPiece->activate();
}

void GameController::update(float deltaTime) {
    m_pieceSystem->update(deltaTime);
    m_collisionSystem->update(deltaTime);
    m_gridSystem->update(deltaTime);
    m_scoreSystem->update(deltaTime);
}

void GameController::resetGame() {
    m_gridSystem->resetBoard();
    m_scoreSystem->resetScore();
    updateNextPiece();
}

void GameController::resumeGame() {

}

void GameController::activate() {
    m_playField->activate();
    m_currentPiece->activate();
    m_nextPiece->activate();
    m_ghostPiece->activate();
}

void GameController::deactivate() {
    m_playField->deactivate();
    m_currentPiece->deactivate();
    m_nextPiece->deactivate();
    m_ghostPiece->deactivate();
}


void GameController::overGame() {
}


