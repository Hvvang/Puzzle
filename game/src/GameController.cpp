#include "GameController.hpp"

#include <MiniKit/MiniKit.hpp>

#include <Systems/SpriteRenderingSystem.hpp>
#include <Systems/GridController.hpp>
#include <Systems/CollisionSystem.hpp>
#include <Systems/PieceController.hpp>
#include <Systems/TileController.hpp>
#include <Systems/ScoreSystem.hpp>

#include <random>

#include <Font/Font.hpp>
#include <Font/Text.hpp>
#include <Font/TextRenderer.hpp>

using ::Engine::Math::operator*=;

GameController::GameController(MiniKit::Engine::Context &context) : m_context(context) {
    m_eventSystem = ::std::make_unique<EventSystem>();

    m_renderingSystem = ::std::make_shared<SpriteRenderingSystem>();
    m_textRenderer = ::std::make_shared<TextRenderer>();
    m_gridSystem = ::std::make_shared<GridController>(this);
    m_collisionSystem = ::std::make_shared<CollisionSystem>(this);
    m_pieceSystem = ::std::make_shared<PieceController>(this);
    m_tileSystem = ::std::make_shared<TileController>(this);
    m_scoreSystem = ::std::make_shared<ScoreSystem>(this);

    /// FOR TEST
    m_levelUp = ::std::make_unique<Entity>(entityManager->createEntity());

    SpriteManager->loadSprite("assets/images/background_b.png", "playField");
    SpriteManager->loadSprite("assets/images/square.png", "square");


    entityManager->addSystem(*m_textRenderer);
    entityManager->addSystem(*m_renderingSystem);
    entityManager->addSystem(*m_gridSystem);
    entityManager->addSystem(*m_collisionSystem);
    entityManager->addSystem(*m_pieceSystem);
    entityManager->addSystem(*m_tileSystem);
    entityManager->addSystem(*m_scoreSystem);

    m_currentPiece = entityManager->createEntity();

    m_playField = ::std::make_unique<Entity>(entityManager->createEntity());
    m_playField->addComponent<BoardComponent>();
    m_playField->addComponent<ScoreComponent>();
    m_playField->activate();
    initPlayFieldBackground();

    m_nextPiece = ::std::make_unique<Entity>(entityManager->createEntity());
    m_nextPiece->addComponent<PieceComponent>();
    m_nextPiece->activate();
    m_levelUp->activate();

    updateNextPiece();
    updatePieces();

    m_eventSystem->connect(this, &GameController::updatePieces);
    m_context.GetWindow().AddResponder(*m_pieceSystem);
}

GameController::~GameController() {
    m_context.GetWindow().RemoveResponder(*m_pieceSystem);
}

void GameController::initPlayFieldBackground() {

    auto &playFieldSprite = m_playField->addComponent<Sprite>();

    playFieldSprite.setImage("playField");
    playFieldSprite.getTransform().scale *= 2.f;
}

void GameController::updatePieces(BlockSetEvent *) {
    if (m_currentPiece.isActivated()) {
        m_currentPiece.erase();
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
    m_currentPiece = entityManager->createEntity();

    auto &pieceComponent = m_currentPiece.addComponent<PieceComponent>();
    auto shape = m_nextPiece->getComponent<PieceComponent>().shape;
    m_currentPiece.addComponent<PieceCollisionComponent>(shape);
    m_currentPiece.addComponent<MoveComponent>();

    m_pieceSystem->spawnPiece(pieceComponent, shape, m_gridSystem->m_spawnLocation);
    m_currentPiece.activate();
}

void GameController::update(float deltaTime) {
    auto& graphicsDevice = m_context.GetGraphicsDevice();
    auto& commandBuffer = graphicsDevice.BeginFrame(1.f, 1.f, 1.f, 1.f);

    m_pieceSystem->update(deltaTime);
    m_collisionSystem->update(deltaTime);
    m_gridSystem->update(deltaTime);
    m_scoreSystem->update(deltaTime);
    m_renderingSystem->update(deltaTime, commandBuffer);
    m_textRenderer->update(deltaTime, commandBuffer);

    graphicsDevice.EndFrame(commandBuffer);
}

