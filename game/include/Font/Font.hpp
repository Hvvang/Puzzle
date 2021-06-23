#pragma once

#include <map>
#include <Font/Glyph.hpp>

struct Glyph;

class Font {
public:
    Font(const ::std::string &texture, const uint32_t rowWidth, const uint32_t glyphSize)
        : m_fontFileName(texture)
        , m_glyphSize(glyphSize)
        , m_rowWidth(rowWidth) { initGlyphTable(); };

    void initGlyphTable();
    const Glyph& getGlyph(uint32_t codePoint) const;

    const ::std::string &getFileName() const noexcept { return m_fontFileName; }
    uint32_t getGlyphSize() const noexcept { return m_glyphSize; }

private:
    uint32_t m_glyphSize;
    uint32_t m_rowWidth;

    ::std::string m_fontFileName;
    ::std::map<uint32_t , Glyph> m_glyphTable;
};

