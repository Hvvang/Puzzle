#pragma once

#include <EntityManager.hpp>
#include <Font/Text.hpp>

#include <array>
#include <memory>

using ::Engine::ECS::Entity;
using ::Engine::ECS::Component;

struct ScoreComponent : Component  {
    ScoreComponent() {
        SpriteManager->loadSprite("assets/images/font_texture.png", "Roboto");
        m_font = ::std::make_unique<Font>("Roboto", 1000, 90);

        level = ::std::make_unique<Entity>(entityManager->createEntity());
        score = ::std::make_unique<Entity>(entityManager->createEntity());
        lines = ::std::make_unique<Entity>(entityManager->createEntity());

        auto &levelText = level->addComponent<Text>(::std::to_string(lvl), *m_font, 7u);
        auto &scoreText = score->addComponent<Text>(::std::to_string(scr), *m_font, 7u);
        auto &linesText = lines->addComponent<Text>(::std::to_string(lns), *m_font, 7u);

        levelText.getTransform().position = {580, 379};
        scoreText.getTransform().position = {580, 458};
        linesText.getTransform().position = {580, 537};

        levelText.getColor() = {240.f / 255.f, 110.f / 255.f, 0.f, 1.f};
        scoreText.getColor() = {240.f / 255.f, 110.f / 255.f, 0.f, 1.f};
        linesText.getColor() = {240.f / 255.f, 110.f / 255.f, 0.f, 1.f};

        level->activate();
        score->activate();
        lines->activate();
    }


    ::std::unique_ptr<Entity> level{ nullptr };
    ::std::unique_ptr<Entity> score{ nullptr };
    ::std::unique_ptr<Entity> lines{ nullptr };

    int lvl = 1;
    int scr = 0;
    int lns = 0;

private:
    ::std::unique_ptr<Font> m_font{ nullptr };
};