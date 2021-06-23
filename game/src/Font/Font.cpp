#include "Font/Font.hpp"

#include <Math.hpp>

using ::Engine::Math::Vector2i;

void Font::initGlyphTable() {
    int pieceInRow = m_rowWidth / m_glyphSize;

    for (uint32_t ch = ' '; ch <= '~'; ++ch) {
        uint32_t x = ((ch - int(' ')) % pieceInRow) ;
        uint32_t y = ((ch - int(' ')) / pieceInRow);

        Glyph glyph;
        glyph.textureRect = {x * m_glyphSize, y * m_glyphSize, m_glyphSize, m_glyphSize};
        m_glyphTable[ch] = glyph;
    }
}

const Glyph &Font::getGlyph(uint32_t codePoint) const {
    return m_glyphTable.at(codePoint);
}
