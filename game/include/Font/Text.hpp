#pragma once

#include "Components/Sprite.hpp"
#include <Font/Font.hpp>

class Font;

using ::Engine::ECS::Sprite;

using ::Engine::Math::Vector2f;

class Text : public Sprite {
public:
    Text(const ::std::string &str, Font &font, unsigned charSize = 30);

    void draw(::MiniKit::Graphics::CommandBuffer &commandBuffer) override;

private:
    Font &m_font;
    ::std::string m_text;
    unsigned m_charSize;
    float m_lineSpacing;
    float m_letterSpacing;
};

