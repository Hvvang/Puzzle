#include "Button.hpp"
#include <EntityManager.hpp>

#include <Font/Text.hpp>
#include <Font/Font.hpp>

Button::Button(const std::string &title, const Vector2f &pos) {
    SpriteManager->loadSprite("assets/images/font_texture.png", "Roboto");
    m_font = ::std::make_unique<Font>("Roboto", 1000, 90);

    m_instance = ::std::make_unique<Entity>(entityManager->createEntity());
    auto &textComponent = m_instance->addComponent<Text>(title, *m_font, 7u);

    textComponent.getTransform().position = pos;
    textComponent.getColor() = {0.f, 0.f, 0.f, 1.f};

    m_instance->activate();
}

void Button::setPosition(const Vector2f &pos) {
    m_instance->getComponent<Text>().getTransform().position = pos;
}

void Button::setTitle(const std::string &title) {
    m_instance->getComponent<Text>().setText(title);
}

void Button::setActive(bool state) {
    if (state) {
        m_instance->getComponent<Text>().getColor() = {240.f / 255.f, 110.f / 255.f, 0.f, 1.f};
    } else {
        m_instance->getComponent<Text>().getColor() = {0.f, 0.f, 0.f, 1.f};
    }
}
