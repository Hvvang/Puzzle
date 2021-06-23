#include "Font/Text.hpp"

#include <Screen.hpp>

using ::Engine::Math::Vector2f;
using ::Engine::Math::operator+;

Text::Text(const std::string &str, Font &font, unsigned int charSize) : m_font(font) {
    m_text = str;
    m_charSize = charSize;
    setImage(font.getFileName());
}

void Text::draw(MiniKit::Graphics::CommandBuffer &commandBuffer) {

    auto &transform = getTransform();
    transform.scale.x = float(m_charSize) / float(m_font.getGlyphSize());
    transform.scale.y = float(m_charSize) / float(m_font.getGlyphSize());

    commandBuffer.SetImage(*getImage());

    ::MiniKit::Graphics::DrawInfo info = {
            getColor(),
            EScreen->screenPointToDrawablePoint(transform.position) + Vector2f{
                    float(m_font.getGlyphSize()) * transform.scale.x / 2.f,
                    -(float(m_font.getGlyphSize()) * transform.scale.y / 2.f)},
            transform.scale,
            transform.angle
    };

    for (char ch : m_text) {
        const auto &glyph = m_font.getGlyph(uint32_t(ch));

        info.position.x += float(m_font.getGlyphSize()) / 2.f;

        commandBuffer.Draw(info, {
            glyph.textureRect.x,
            glyph.textureRect.y,
            glyph.textureRect.width,
            glyph.textureRect.height
        });
    }

}
