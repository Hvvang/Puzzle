#pragma once

#include <EntityComponentSystem.hpp>
#include <Components/ScoreComponent.hpp>
#include <GameController.hpp>


using ::Engine::ECS::System;
using ::Engine::ECS::Requires;

class GameController;
class ScoreComponent;

class LinesClearEvent;
class SoftDropEvent;


class ScoreSystem : public System<Requires<ScoreComponent>> {
public:
    explicit ScoreSystem(GameController *parent = nullptr);;

    void resetScore();

    void update(float deltaTime);

    void onLinesClear(LinesClearEvent *);
    void onSoftDrop(SoftDropEvent *);

    void activate();

    void deactivate();

private:
    GameController *m_parent = { nullptr };
    int m_clearLines{0};

};




