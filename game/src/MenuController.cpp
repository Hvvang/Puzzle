#include "MenuController.hpp"
#include <EntityManager.hpp>
#include <Components/Sprite.hpp>

#include <imgui.h>

using ::Engine::ECS::Sprite;
using ::Engine::Math::operator*=;

MenuController::MenuController(MiniKit::Engine::Context &context) : m_context(context), currentPage(Menu::NewGame) {
    SpriteManager->loadSprite("assets/images/Menu.png", "menu_background");

    m_background = ::std::make_unique<Entity>(entityManager->createEntity());
    auto &spriteComponent = m_background->addComponent<Sprite>();
    spriteComponent.setImage("menu_background");
    spriteComponent.getTransform().scale *= 2.f;
    m_background->activate();

    m_menuButtons.push_back(::std::make_unique<Button>("New Game", Vector2f{260.f, 300.f}));
    m_menuButtons.push_back(::std::make_unique<Button>("Resume", Vector2f{280.f, 360.f}));
    m_menuButtons.push_back(::std::make_unique<Button>("Settings", Vector2f{260.f, 420.f}));
    m_menuButtons.push_back(::std::make_unique<Button>("Quit", Vector2f{300.f, 480.f}));

    m_menuButtons.at(0)->setActive(true);
}

void MenuController::update(float) {
    if (open) {
        switch (currentPage) {
            case Menu::NewGame:
                break;
            case Menu::Resume:
                break;
            case Menu::Settings:
                ::ImGui::Begin("Settings", NULL,
                               ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

                ::ImGui::Checkbox("Ghost piece", &_ghostPiece);
                ::ImGui::Checkbox("Animation", &_animation);
                ::ImGui::End();
                break;
            case Menu::Quit:
                m_context.Terminate();
                break;
        }
    }

}

void MenuController::KeyDown(Window &window, const KeyEvent &event) noexcept {
    now = ::std::chrono::duration_cast<::std::chrono::milliseconds>(::std::chrono::system_clock::now().time_since_epoch());

    if (now - inputDelay > std::chrono::milliseconds(130)) {
        switch (event.keycode) {
            case MiniKit::Platform::Keycode::KeyEnter:
                open = !open;
                break;
            case MiniKit::Platform::Keycode::KeyUp:
                if (currentPage > Menu::NewGame && !open) {
                    m_menuButtons.at(currentPage)->setActive(false);
                    currentPage = static_cast<Menu>(currentPage - 1);
                    m_menuButtons.at(currentPage)->setActive(true);
                }
                break;
            case MiniKit::Platform::Keycode::KeyDown:
                if (currentPage < Menu::Quit && !open) {
                    m_menuButtons.at(currentPage)->setActive(false);
                    currentPage = static_cast<Menu>(currentPage + 1);
                    m_menuButtons.at(currentPage)->setActive(true);
                }
                break;
            case MiniKit::Platform::Keycode::KeyEscape:
                open = false;
                break;
        }
        inputDelay = now;
    }
}


