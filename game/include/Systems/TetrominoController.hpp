#pragma once

#include <EntityComponentSystem.hpp>
#include <Components/PieceComponent.hpp>

#include <MiniKit/MiniKit.hpp>

using ::Engine::ECS::System;
using ::Engine::ECS::Requires;

struct TetrominoController : System<Requires<PieceComponent>>, ::MiniKit::Platform::Responder {
    void update(float deltaTime) {
        m_localTime += deltaTime;
        moveTime += deltaTime;

        auto entities = getEntities();
        for (auto &entity : entities) {
            auto &tetromino = entity.getComponent<PieceComponent>();

            if (moveTime >= 0.12f) {
                tetromino.move({int(m_leftOffset), 0});
                tetromino.move({int(m_RightOffset), 0});
                moveTime = 0.f;
            }

            if (m_localTime >= tetromino.getSpeed()) {

                tetromino.move({0, 1});
                m_localTime = 0.f;
            }
        }
    }

    void KeyDown(::MiniKit::Platform::Window &window, const ::MiniKit::Platform::KeyEvent &event) noexcept override {
        if (event.keycode == ::MiniKit::Platform::Keycode::KeyLeft)
        {
            m_leftOffset = -1.0f;
        }

        if (event.keycode == ::MiniKit::Platform::Keycode::KeyRight)
        {
            m_RightOffset = 1.0f;
        }

        if (event.keycode == ::MiniKit::Platform::Keycode::KeyDown)
        {
            m_DownOffset = -1.0f;
        }
    }

    void KeyUp(::MiniKit::Platform::Window &window, const ::MiniKit::Platform::KeyEvent &event) noexcept override {
        if (event.keycode == ::MiniKit::Platform::Keycode::KeyLeft)
        {
            m_leftOffset = 0.0f;
        }

        if (event.keycode == ::MiniKit::Platform::Keycode::KeyRight)
        {
            m_RightOffset = 0.f;
        }

        if (event.keycode == ::MiniKit::Platform::Keycode::KeyDown)
        {
            m_DownOffset = 0.f;
        }
    }

    float m_localTime = 0.f;
    float moveTime = 0.f;
    float m_leftOffset = 0.f;
    float m_RightOffset = 0.f;
    float m_DownOffset = 0.f;
};