#include "App.hpp"
#include <Systems/SpriteRenderingSystem.hpp>
#include <Font/TextRenderer.hpp>
#include <Settings.hpp>

::std::error_code App::Start(MiniKit::Engine::Context &context) noexcept {
    m_screen = ::std::make_unique<Screen>(context);
    m_entityManager = ::std::make_unique<EntityManager>();
    m_spriteManager = ::std::make_unique<SpriteLoader>(context);
    m_menuManager = ::std::make_unique<MenuController>(this, context);
    m_gameManager = ::std::make_unique<GameController>(this, context);
    m_renderingSystem = ::std::make_shared<SpriteRenderingSystem>();
    m_textRenderer = ::std::make_shared<TextRenderer>();

    entityManager->addSystem(*m_textRenderer);
    entityManager->addSystem(*m_renderingSystem);

    context.GetWindow().AddResponder(*m_menuManager);

    return std::error_code();
}

::std::error_code App::Shutdown(MiniKit::Engine::Context &context) noexcept {
    return ::MiniKit::MakeErrorCode(::MiniKit::Error::None);
}

void App::Tick(MiniKit::Engine::Context &context) noexcept {
    auto& graphicsDevice = context.GetGraphicsDevice();
    auto& commandBuffer = graphicsDevice.BeginFrame(1.f, 1.f, 1.f, 1.f);

    m_entityManager->refresh();

    switch (m_currentState) {
        case State::Game:
            m_gameManager->update(context.GetFrameDelta());
            break;
        case State::Menu:
            m_menuManager->update(context.GetFrameDelta());
            break;
    }

    m_renderingSystem->update(context.GetFrameDelta(), commandBuffer);
    m_textRenderer->update(context.GetFrameDelta(), commandBuffer);

    graphicsDevice.EndFrame(commandBuffer);
}

void App::ChangeState() {

    if (m_currentState == State::Menu) {
        m_menuManager->deactivate();
        if (m_menuManager->currentPage == MenuController::NewGame) {
            m_gameManager->NewGameState();
        } else {
            m_gameManager->ResumeGameState();
        }

    } else {
        m_gameManager->deactivate();
        m_menuManager->activate();
    }
    m_currentState = m_currentState == State::Menu ? State::Game : State::Menu;
}
