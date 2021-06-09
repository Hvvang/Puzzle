#include "App.hpp"

#include <EntityManager.hpp>
#include <Components/Sprite.hpp>

#include <iostream>

using ::Engine::ECS::Sprite;


::std::error_code App::Start(MiniKit::Engine::Context &context) noexcept {

    m_screen = ::std::make_unique<Screen>(context);
    m_entityManager = ::std::make_unique<EntityManager>();
    m_spriteManager = ::std::make_unique<SpriteLoader>(context);
    m_renderingSystem = ::std::make_shared<SpriteRenderingSystem>(context);
    m_tetrominoSystem = ::std::make_shared<TetrominoController>();

    context.GetWindow().AddResponder(*m_tetrominoSystem);

    {
        m_spriteManager->loadSprite("assets/images/background_g.png", "playField");
        m_spriteManager->loadSprite("assets/images/square.png", "square");
    }

    m_playField = ::std::make_unique<Entity>(m_entityManager->createEntity());
    auto &playFieldSprite = m_playField->addComponent<Sprite>();
    auto &transform = playFieldSprite.getTransform();

    playFieldSprite.setImage("playField");

    transform.scale.x *= 2;
    transform.scale.y *= 2;

    m_playField->activate();

    m_tetromino = ::std::make_unique<Entity>(m_entityManager->createEntity());
    m_tetromino->addComponent<TetrominoShape>(TetrominoShape::Shape::IShape);
    m_tetromino->activate();

    m_entityManager->addSystem(*m_tetrominoSystem);
    m_entityManager->addSystem(*m_renderingSystem);
    return std::error_code();
}

::std::error_code App::Shutdown(MiniKit::Engine::Context &context) noexcept {
    return ::MiniKit::MakeErrorCode(::MiniKit::Error::None);
}

void App::Tick(MiniKit::Engine::Context &context) noexcept {
    m_globalTime += context.GetFrameDelta();

    if (m_globalTime >= 8.f) {
        m_tetromino->deactivate();
    }

    m_entityManager->refresh();

    m_tetrominoSystem->update(context.GetFrameDelta());

    m_renderingSystem->update(context.GetFrameDelta());
}
