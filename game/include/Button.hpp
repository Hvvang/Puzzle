#pragma once

#include <EntityComponentSystem.hpp>
#include <Math.hpp>

#include <Font/Font.hpp>

namespace Engine::ECS {
    class Entity;
}

using ::Engine::ECS::Entity;
using ::Engine::Math::Vector2f;

class Button {
public:
    Button(const std::string &title, const Vector2f &pos);

    void setPosition(const Vector2f &pos);
    void setTitle(const ::std::string &title);
    void setActive(bool state);

private:
    ::std::unique_ptr<Font> m_font{ nullptr };
    ::std::unique_ptr<Entity> m_instance{ nullptr };
};


