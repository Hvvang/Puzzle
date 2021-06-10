#include "App.hpp"

#include <EntityManager.hpp>

#include <Components/Sprite.hpp>
#include <Components/BoardComponent.hpp>

#include <Systems/SpriteRenderingSystem.hpp>
#include <Systems/BoardController.hpp>

#include <iostream>

using ::Engine::ECS::Sprite;

::std::error_code App::Start(MiniKit::Engine::Context &context) noexcept {

    m_screen = ::std::make_unique<Screen>(context);
    m_entityManager = ::std::make_unique<EntityManager>();
    m_spriteManager = ::std::make_unique<SpriteLoader>(context);

    {
        m_spriteManager->loadSprite("assets/images/background_b.png", "playField");
        m_spriteManager->loadSprite("assets/images/square.png", "square");
    }

    m_renderingSystem = ::std::make_shared<SpriteRenderingSystem>(context);

    m_playField = ::std::make_unique<Entity>(m_entityManager->createEntity());
    auto &board = m_playField->addComponent<BoardComponent>();
    auto &playFieldSprite = m_playField->addComponent<Sprite>();
    auto &transform = playFieldSprite.getTransform();

    playFieldSprite.setImage("playField");

    transform.scale.x *= 2;
    transform.scale.y *= 2;

    m_playField->activate();
    m_boardSystem = ::std::make_shared<BoardController>(context);

    m_entityManager->addSystem(*m_renderingSystem);
    m_entityManager->addSystem(*m_boardSystem);

    return std::error_code();
}

::std::error_code App::Shutdown(MiniKit::Engine::Context &context) noexcept {
    return ::MiniKit::MakeErrorCode(::MiniKit::Error::None);
}

void App::Tick(MiniKit::Engine::Context &context) noexcept {
    m_entityManager->refresh();

    m_boardSystem->update(context.GetFrameDelta());

    m_renderingSystem->update(context.GetFrameDelta());
}
