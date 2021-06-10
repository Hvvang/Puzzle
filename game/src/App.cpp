#include "App.hpp"

::std::error_code App::Start(MiniKit::Engine::Context &context) noexcept {

    m_screen = ::std::make_unique<Screen>(context);
    m_entityManager = ::std::make_unique<EntityManager>();
    m_spriteManager = ::std::make_unique<SpriteLoader>(context);
    m_gameController = ::std::make_unique<GameController>(context);

    return std::error_code();
}

::std::error_code App::Shutdown(MiniKit::Engine::Context &context) noexcept {
    return ::MiniKit::MakeErrorCode(::MiniKit::Error::None);
}

void App::Tick(MiniKit::Engine::Context &context) noexcept {
    m_entityManager->refresh();
    m_gameController->update(context.GetFrameDelta());
}
