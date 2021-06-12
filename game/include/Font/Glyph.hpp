#pragma once

#include <Math.hpp>

using ::Engine::Math::FloatRect;
using ::Engine::Math::UIntRect;

struct Glyph {
    Glyph() : advance(0) {};

    float advance;
    FloatRect bounds;
    UIntRect textureRect;
};