#pragma once

#include <MiniKit/MiniKit.hpp>

#include <EntityManager.hpp>
#include <SpriteLoader.hpp>


#include <Screen.hpp>



namespace Engine::ECS {
    class SpriteRenderingSystem;
};
class BoardController;

using ::Engine::View::Screen;
using ::Engine::ECS::EntityManager;
using ::Engine::ECS::Entity;
using ::Engine::Loader::SpriteLoader;
using ::Engine::ECS::SpriteRenderingSystem;

class App final : public ::MiniKit::Engine::Application, public ::MiniKit::Platform::Responder {
public:
    ::std::error_code Start(::MiniKit::Engine::Context& context) noexcept override;

    ::std::error_code Shutdown(::MiniKit::Engine::Context& context) noexcept override;

    void Tick(::MiniKit::Engine::Context& context) noexcept override;


private:
    ::std::unique_ptr<Screen> m_screen{nullptr};
    ::std::unique_ptr<SpriteLoader> m_spriteManager{nullptr};
    ::std::unique_ptr<EntityManager> m_entityManager{nullptr};

    ::std::shared_ptr<SpriteRenderingSystem> m_renderingSystem{nullptr};
    ::std::shared_ptr<BoardController> m_boardSystem{nullptr};

    ::std::unique_ptr<Entity> m_playField{nullptr};
};


