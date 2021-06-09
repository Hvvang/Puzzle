#pragma once

#include <MiniKit/MiniKit.hpp>

#include <EntityManager.hpp>
#include <SpriteLoader.hpp>

#include <Systems/SpriteRenderingSystem.hpp>

#include <Screen.hpp>

#include <Systems/TetrominoController.hpp>
#include <Components/PieceComponent.hpp>

#include <Components/BoardComponent.hpp>

using ::Engine::View::Screen;
using ::Engine::ECS::EntityManager;
using ::Engine::ECS::Entity;
using ::Engine::ECS::SpriteRenderingSystem;
using ::Engine::Loader::SpriteLoader;

class App final : public ::MiniKit::Engine::Application, public ::MiniKit::Platform::Responder {
public:
    ::std::error_code Start(::MiniKit::Engine::Context& context) noexcept override;

    ::std::error_code Shutdown(::MiniKit::Engine::Context& context) noexcept override;

    void Tick(::MiniKit::Engine::Context& context) noexcept override;


private:
    float m_globalTime = 0.f;

    ::std::unique_ptr<Screen> m_screen{nullptr};
    ::std::unique_ptr<SpriteLoader> m_spriteManager{nullptr};
    ::std::unique_ptr<EntityManager> m_entityManager{nullptr};

    ::std::shared_ptr<SpriteRenderingSystem> m_renderingSystem{nullptr};
    ::std::shared_ptr<TetrominoController> m_tetrominoSystem{nullptr};


    ::std::unique_ptr<Entity> m_playField{nullptr};
    ::std::unique_ptr<Entity> m_tetromino{nullptr};
};


