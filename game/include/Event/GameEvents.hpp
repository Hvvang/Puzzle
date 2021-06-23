#pragma once

#include <Event/Event.hpp>

struct BlockSetEvent : Event {};

struct SoftDropEvent : Event {};
struct SoftDropCollidedEvent : Event {};
struct LinesClearEvent : Event {
    LinesClearEvent(::std::vector<int> data) : lines(data) {}

    ::std::vector<int> lines{0};
};

struct LevelUpEvent : Event{};