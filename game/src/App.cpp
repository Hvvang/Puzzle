#include "App.hpp"
#include <Systems/SpriteRenderingSystem.hpp>
#include <Font/TextRenderer.hpp>

::std::error_code App::Start(MiniKit::Engine::Context &context) noexcept {
    m_screen = ::std::make_unique<Screen>(context);
    m_entityManager = ::std::make_unique<EntityManager>();
    m_spriteManager = ::std::make_unique<SpriteLoader>(context);
    m_menuManager = ::std::make_unique<MenuController>(context);
//    m_gameManager = ::std::make_unique<GameController>(context);
    m_renderingSystem = ::std::make_shared<SpriteRenderingSystem>();
    m_textRenderer = ::std::make_shared<TextRenderer>();

    entityManager->addSystem(*m_textRenderer);
    entityManager->addSystem(*m_renderingSystem);

    context.GetWindow().AddResponder(*m_menuManager);

    return std::error_code();
}

::std::error_code App::Shutdown(MiniKit::Engine::Context &context) noexcept {
    context.GetWindow().RemoveResponder(*m_menuManager);
    return ::MiniKit::MakeErrorCode(::MiniKit::Error::None);
}

void App::Tick(MiniKit::Engine::Context &context) noexcept {
    auto& graphicsDevice = context.GetGraphicsDevice();
    auto& commandBuffer = graphicsDevice.BeginFrame(1.f, 1.f, 1.f, 1.f);

    m_entityManager->refresh();

//    if (isGame) {
//        m_gameManager->update(context.GetFrameDelta());
//    } else {
        m_menuManager->update(context.GetFrameDelta());
//    }

    m_renderingSystem->update(context.GetFrameDelta(), commandBuffer);
    m_textRenderer->update(context.GetFrameDelta(), commandBuffer);

    graphicsDevice.EndFrame(commandBuffer);
}
