#pragma once

#include <MiniKit/MiniKit.hpp>

#include <GameController.hpp>
#include <MenuController.hpp>

#include <EntityManager.hpp>
#include <SpriteLoader.hpp>
#include <Screen.hpp>

class TextRenderer;
namespace Engine::ECS {
    class SpriteRenderingSystem;
}
using ::Engine::ECS::SpriteRenderingSystem;
using ::Engine::View::Screen;
using ::Engine::ECS::EntityManager;
using ::Engine::Loader::SpriteLoader;

class App final : public ::MiniKit::Engine::Application {

    enum class State {
        Menu,
        Game
    } m_currentState{State::Menu};

public:

    ::std::error_code Start(::MiniKit::Engine::Context &context) noexcept override;

    ::std::error_code Shutdown(::MiniKit::Engine::Context &context) noexcept override;

    void Tick(::MiniKit::Engine::Context &context) noexcept override;

    void ChangeState();


private:
    bool isGame = false;

    ::std::unique_ptr<Screen> m_screen{nullptr};

    ::std::unique_ptr<SpriteLoader> m_spriteManager{nullptr};
    ::std::unique_ptr<EntityManager> m_entityManager{nullptr};

    ::std::unique_ptr<MenuController> m_menuManager{nullptr};
    ::std::unique_ptr<GameController> m_gameManager{nullptr};

    ::std::shared_ptr<SpriteRenderingSystem> m_renderingSystem{ nullptr };
    ::std::shared_ptr<TextRenderer> m_textRenderer{ nullptr };
};

