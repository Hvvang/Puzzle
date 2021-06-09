#pragma once

#include <EntityComponentSystem.hpp>
#include <Components/PieceComponent.hpp>

using ::Engine::ECS::System;
using ::Engine::ECS::Requires;

class PieceController : public System<Requires<PieceComponent>> {
public:

    void initPiece() {

    }

};


