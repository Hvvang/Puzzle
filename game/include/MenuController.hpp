#pragma once

#include <EntityComponentSystem.hpp>
#include <Utils/Button.hpp>

class App;

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
        SettingsMenu = 2,
        Quit = 3
    } currentPage{NewGame};

    explicit MenuController(App *parent, MiniKit::Engine::Context &context);

    void update(float deltaTime);

    void KeyDown(Window &window, const KeyEvent &event) noexcept override;

    void deactivate();

    void activate();

private:
    App *m_parent{ nullptr };
    MiniKit::Engine::Context &m_context;

    bool open = false;

    ::std::chrono::milliseconds now;
    ::std::chrono::milliseconds inputDelay{0};

    ::std::unique_ptr<Entity> m_background{ nullptr };

    ::std::vector<::std::unique_ptr<Button>> m_menuButtons;

};

