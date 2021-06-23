#pragma once

#include <EntityManager.hpp>
#include <Math.hpp>

#include <Font/Text.hpp>
#include <Font/Font.hpp>

namespace Engine::ECS {
    class Entity;
}

using ::Engine::ECS::Entity;
using ::Engine::Math::Vector2f;

class Button {
public:
    Button(const std::string &title, const Vector2f &pos) {
        SpriteManager->loadSprite("assets/images/font_texture.png", "Roboto");
        m_font = ::std::make_unique<Font>("Roboto", 1000, 90);

        m_instance = ::std::make_unique<Entity>(entityManager->createEntity());
        auto &textComponent = m_instance->addComponent<Text>(title, *m_font, 7u);

        textComponent.getTransform().position = pos;
        textComponent.getColor() = {0.f, 0.f, 0.f, 1.f};

        m_instance->activate();
    }

    void setPosition(const Vector2f &pos) {
        m_instance->getComponent<Text>().getTransform().position = pos;
    }
    void setTitle(const ::std::string &title) {
        m_instance->getComponent<Text>().setText(title);
    }
    void setState(bool state) {
        if (state) {
            m_instance->getComponent<Text>().getColor() = {240.f / 255.f, 110.f / 255.f, 0.f, 1.f};
        } else {
            m_instance->getComponent<Text>().getColor() = {0.f, 0.f, 0.f, 1.f};
        }
    }

    void activate() {
        m_instance->activate();
    }

    void deactivate() {
        m_instance->deactivate();
    }

private:
    ::std::unique_ptr<Font> m_font{ nullptr };
    ::std::unique_ptr<Entity> m_instance{ nullptr };
};


