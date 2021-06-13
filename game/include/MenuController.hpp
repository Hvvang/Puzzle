#pragma once

#include <EntityComponentSystem.hpp>
#include <Button.hpp>

namespace Engine::ECS {
    class Entity;
    class SpriteRenderingSystem;
}

using ::MiniKit::Platform::Responder;
using ::MiniKit::Platform::Window;
using ::MiniKit::Platform::KeyEvent;

using ::Engine::ECS::Entity;
using ::Engine::ECS::SpriteRenderingSystem;

class MenuController : public Responder  {
public:
    enum Menu {
        NewGame = 0,
        Resume = 1,
        Settings = 2,
        Quit = 3
    } currentPage;

    explicit MenuController(MiniKit::Engine::Context &context);

    void update(float deltaTime);

    void KeyDown(Window &window, const KeyEvent &event) noexcept override;

private:
    MiniKit::Engine::Context &m_context;

    bool open = false;
    bool _ghostPiece = true;
    bool _animation = true;

    ::std::chrono::milliseconds now;
    ::std::chrono::milliseconds inputDelay{0};

    ::std::unique_ptr<Entity> m_background{ nullptr };

    ::std::vector<::std::unique_ptr<Button>> m_menuButtons;

};

