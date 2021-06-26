#include "Systems/ScoreSystem.hpp"
#include <GameController.hpp>

ScoreSystem::ScoreSystem(GameController *parent) : m_parent(parent) {
    m_parent->m_eventSystem->connect(this, &ScoreSystem::onLinesClear);
    m_parent->m_eventSystem->connect(this, &ScoreSystem::onSoftDrop);
    m_parent->m_eventSystem->connect(this, &ScoreSystem::onHardDrop);
}

void ScoreSystem::update(float deltaTime) {
    auto entities = getEntities();
    for (auto &entity : entities) {
        auto &score = entity.getComponent<ScoreComponent>();

        score.level->getComponent<Text>().setText(::std::to_string(score.lvl));
        score.lines->getComponent<Text>().setText(::std::to_string(score.lns));
        if (score.scr > 99999) {
            score.score->getComponent<Text>().setText(::std::to_string(score.scr / 1000000) + "K");
        } else {
            score.score->getComponent<Text>().setText(::std::to_string(score.scr));
        }
    }
}

void ScoreSystem::onLinesClear(LinesClearEvent *e) {
    auto entities = getEntities();
    for (auto &entity : entities) {
        auto &score = entity.getComponent<ScoreComponent>();

        int resScore = 0;

        switch (e->_lines.size()) {
            case 1:
                resScore = 100; break;
            case 2:
                resScore = 300; break;
            case 3:
                resScore = 500; break;
            case 4:
                resScore = 800; break;
        }

        score.scr += resScore * score.lvl;
        score.lns += e->_lines.size();
        m_clearLines += e->_lines.size();

        if (m_clearLines >= 5 + (3 * (score.lvl - 1))) {
            score.lvl += 1;
            m_clearLines = 0;
            m_parent->m_eventSystem->emit(new LevelUpEvent);
        }
    }
}

void ScoreSystem::onSoftDrop(SoftDropEvent *e) {
    auto entities = getEntities();
    for (auto &entity : entities) {
        auto &score = entity.getComponent<ScoreComponent>();

        score.scr += 1;
    }
}

void ScoreSystem::onHardDrop(HardDropEvent *e) {
    auto entities = getEntities();
    for (auto &entity : entities) {
        auto &score = entity.getComponent<ScoreComponent>();

        score.scr += e->_distance * 2;
    }
}

void ScoreSystem::resetScore() {
    auto entities = getEntities();
    for (auto &entity : entities) {
        auto &score = entity.getComponent<ScoreComponent>();

        score.lvl = 1;
        score.scr = 0;
        score.lns = 0;
    }
}

void ScoreSystem::activate() {
    auto entities = getEntities();
    for (auto &entity : entities) {
        entity.getComponent<ScoreComponent>().activate();
    }
}

void ScoreSystem::deactivate() {
    auto entities = getEntities();
    for (auto &entity : entities) {
        entity.getComponent<ScoreComponent>().deactivate();
    }
}


