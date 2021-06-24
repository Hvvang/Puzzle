#pragma once

namespace Engine::ECS {
    class Component;
}

using ::Engine::ECS::Component;

struct MoveComponent : Component {

    MoveComponent() : state(State::MoveDown) {}

    enum State : uint8_t {
        MoveLeft = 0x1,
        MoveRight = 0x2,
        MoveDown = 0x4,
        SoftDownMove = 0x8,
        HardDownMove = 0x10,
        RotateLeft = 0x20,
        RotateRight = 0x40,

    } state;

    struct Data {
        State state;
        int rotationIndex;
    } previousData;

    int rotationIndex{0};
    float m_speed = 0.6f;

    ::std::unordered_map<State, Vector2i> direction = {
            {MoveLeft, {-1, 0}},
            {MoveRight, {1, 0}},
            {MoveDown, {0, 1}},
            {SoftDownMove, {0, 1}},
            {HardDownMove, {0, 0}},
    };
};