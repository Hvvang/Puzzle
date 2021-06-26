#include "MenuController.hpp"
#include <EntityManager.hpp>
#include <Components/Sprite.hpp>
#include <Settings.hpp>
#include <App.hpp>
#include <imgui.h>

using ::Engine::ECS::Sprite;
using ::Engine::Math::operator*=;

const char* themes[] = { "Blue", "Grey", "Purple" };

MenuController::MenuController(App *parent, MiniKit::Engine::Context &context) : m_parent(parent), m_context(context) {
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

    m_menuButtons.at(0)->setState(true);
}

void MenuController::update(float) {
    if (open) {
        bool _animation = settings->getValue("Animation");
        bool _ghostPiece = settings->getValue("Ghost piece");
        int themeIndex = settings->getValue("Theme");
        float _pieceColor[4] = {settings->getValue("Block set color")[0],
                                settings->getValue("Block set color")[1],
                                settings->getValue("Block set color")[2],
                                settings->getValue("Block set color")[3]};

        switch (currentPage) {
            case Menu::NewGame:
                m_parent->ChangeState();
                open = false;
                break;
            case Menu::Resume:
                m_parent->ChangeState();
                open = false;
                break;
            case Menu::SettingsMenu:
                ::ImGui::Begin("Settings", NULL,
                               ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

                ::ImGui::Combo("Theme", &themeIndex, themes, IM_ARRAYSIZE(themes));
                ::ImGui::Checkbox("Animation", &_animation);
                ::ImGui::Checkbox("Ghost piece", &_ghostPiece);
                ::ImGui::ColorEdit4("Piece set color", (float*)&_pieceColor);

                settings->setValue("Theme", themeIndex);
                settings->setValue("Ghost piece", _ghostPiece);
                settings->setValue("Animation", _animation);
                settings->setValue("Block set color", {_pieceColor[0], _pieceColor[1], _pieceColor[2], _pieceColor[3]});
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
                    m_menuButtons.at(currentPage)->setState(false);
                    currentPage = static_cast<Menu>(currentPage - 1);
                    if (currentPage == Menu::Resume && !m_parent->hasActiveGame()) {
                        currentPage = static_cast<Menu>(currentPage - 1);
                    }
                    m_menuButtons.at(currentPage)->setState(true);
                }
                break;
            case MiniKit::Platform::Keycode::KeyDown:
                if (currentPage < Menu::Quit && !open) {
                    m_menuButtons.at(currentPage)->setState(false);
                    currentPage = static_cast<Menu>(currentPage + 1);
                    if (currentPage == Menu::Resume && !m_parent->hasActiveGame()) {
                        currentPage = static_cast<Menu>(currentPage + 1);
                    }
                    m_menuButtons.at(currentPage)->setState(true);
                }
                break;
            case MiniKit::Platform::Keycode::KeyEscape:
                open = false;
                break;
        }
        inputDelay = now;
    }
}

void MenuController::deactivate() {
    m_context.GetWindow().RemoveResponder(*this);
    m_background->deactivate();
    for (auto &button : m_menuButtons) {
        button->deactivate();
    }
}

void MenuController::activate() {
    m_context.GetWindow().AddResponder(*this);
    m_background->activate();
    for (auto &button : m_menuButtons) {
        button->activate();
    }
}


