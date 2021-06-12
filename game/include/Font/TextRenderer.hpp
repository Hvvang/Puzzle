#pragma once

#include <Font/Text.hpp>

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

