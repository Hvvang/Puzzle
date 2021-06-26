#pragma once

#include <Event/Event.hpp>

struct PieceFallenEvent : Event{};
struct BlockSetEvent : Event {};
struct SpawnPieceEvent : Event {};

struct SoftDropEvent : Event {};
struct HardDropEvent : Event {
    HardDropEvent(uint8_t data) : _distance(data) {}

    uint8_t _distance{0};
};

struct LinesClearEvent : Event {
    LinesClearEvent(::std::vector<int> data) : _lines(data) {}

    ::std::vector<int> _lines{0};
};

struct LevelUpEvent : Event{};
struct GameOverEvent : Event{};