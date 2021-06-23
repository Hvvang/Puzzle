#pragma once

#include <Font/Text.hpp>

using ::Engine::ECS::System;
using ::Engine::ECS::Requires;

class TextRenderer : public System<Requires<Text>> {
public:
    void update(float deltaTime, CommandBuffer &commandBuffer) {
        auto entities = getEntities();
        for (auto &entity : entities) {
            auto &text = entity.getComponent<Text>();

            text.draw(commandBuffer);
        }
    }
};

