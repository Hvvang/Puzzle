#include "GameController.hpp"

#include <MiniKit/MiniKit.hpp>

#include <Systems/SpriteRenderingSystem.hpp>
#include <Systems/GridController.hpp>
#include <Systems/CollisionSystem.hpp>
#include <Systems/PieceController.hpp>
#include <Systems/TileController.hpp>

#include <random>

using ::Engine::Math::operator*=;

GameController::GameController(MiniKit::Engine::Context &context) {
    m_eventSystem = ::std::make_unique<EventSystem>();

    m_renderingSystem = ::std::make_shared<SpriteRenderingSystem>(context);
    m_gridSystem = ::std::make_shared<GridController>(this);
    m_collisionSystem = ::std::make_shared<CollisionSystem>(this);
    m_pieceSystem = ::std::make_shared<PieceController>(this);
    m_tileSystem = ::std::make_shared<TileController>(this);

    context.GetWindow().AddResponder(*m_pieceSystem);

    entityManager->addSystem(*m_renderingSystem);
    entityManager->addSystem(*m_gridSystem);
    entityManager->addSystem(*m_collisionSystem);
    entityManager->addSystem(*m_pieceSystem);
    entityManager->addSystem(*m_tileSystem);

    m_currentPiece = entityManager->createEntity();

    m_playField = ::std::make_unique<Entity>(entityManager->createEntity());
    m_playField->addComponent<BoardComponent>();
    initPlayFieldBackground();

    m_playField->activate();
    spawnPiece();

    m_eventSystem->connect(this, &GameController::spawnPiece);
}

void GameController::initPlayFieldBackground() {
    SpriteManager->loadSprite("assets/images/background_b.png", "playField");
    SpriteManager->loadSprite("assets/images/square.png", "square");

    auto &playFieldSprite = m_playField->addComponent<Sprite>();

    playFieldSprite.setImage("playField");
    playFieldSprite.getTransform().scale *= 2.f;
}

void GameController::spawnPiece(BlockSetEvent *) {
    if (m_currentPiece.isActivated()) {
        m_currentPiece.erase();
    }
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distribution(0, 6);

    auto shape = static_cast<PieceComponent::Shape>(distribution(gen));

    m_currentPiece = entityManager->createEntity();
    auto &pieceComponent = m_currentPiece.addComponent<PieceComponent>();
    m_currentPiece.addComponent<PieceCollisionComponent>(shape);

    m_pieceSystem->spawnPiece(pieceComponent, shape, m_gridSystem->m_spawnLocation);
    m_currentPiece.activate();
}

void GameController::update(float deltaTime) {
    m_pieceSystem->update(deltaTime);
    m_collisionSystem->update(deltaTime);
    m_gridSystem->update(deltaTime);
    m_renderingSystem->update(deltaTime);
}

