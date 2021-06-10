#pragma once

#include <MiniKit/MiniKit.hpp>

#include <GameController.hpp>
#include <EntityManager.hpp>
#include <SpriteLoader.hpp>
#include <Screen.hpp>

using ::Engine::View::Screen;
using ::Engine::ECS::EntityManager;
using ::Engine::Loader::SpriteLoader;

class App final : public ::MiniKit::Engine::Application, public ::MiniKit::Platform::Responder {
public:
    ::std::error_code Start(::MiniKit::Engine::Context &context) noexcept override;

    ::std::error_code Shutdown(::MiniKit::Engine::Context &context) noexcept override;

    void Tick(::MiniKit::Engine::Context &context) noexcept override;


private:
    ::std::unique_ptr<Screen> m_screen{nullptr};
    ::std::unique_ptr<SpriteLoader> m_spriteManager{nullptr};
    ::std::unique_ptr<EntityManager> m_entityManager{nullptr};

    ::std::unique_ptr<GameController> m_gameController{nullptr};
};

