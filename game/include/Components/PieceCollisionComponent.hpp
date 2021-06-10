#pragma once

#include <EntityManager.hpp>
#include <Math.hpp>
#include <Components/PieceComponent.hpp>

using ::Engine::ECS::Component;
using ::Engine::Math::Vector2i;

struct PieceCollisionComponent : Component {
    PieceCollisionComponent(PieceComponent::Shape shape) {
        if (shape == PieceComponent::Shape::I) {
            offsetMatrix[0][0] = {0, 0};
            offsetMatrix[0][1] = {-1, 0};
            offsetMatrix[0][2] = {-1, -1};
            offsetMatrix[0][3] = {0, -1};

            offsetMatrix[1][0] = {-1, 0};
            offsetMatrix[1][1] = {0, 0};
            offsetMatrix[1][2] = {1, -1};
            offsetMatrix[1][3] = {0, -1};

            offsetMatrix[2][0] = {2, 0};
            offsetMatrix[2][1] = {0, 0};
            offsetMatrix[2][2] = {-2, -1};
            offsetMatrix[2][3] = {0, -1};

            offsetMatrix[3][0] = {-1, 0};
            offsetMatrix[3][1] = {0, -1};
            offsetMatrix[3][2] = {1, 0};
            offsetMatrix[3][3] = {0, 1};

            offsetMatrix[4][0] = {2, 0};
            offsetMatrix[4][1] = {0, 2};
            offsetMatrix[4][2] = {-2, 0};
            offsetMatrix[4][3] = {0, -2};
        } else if (shape == PieceComponent::Shape::O) {
            offsetMatrix[0][0] = {0, 0};
            offsetMatrix[0][1] = {0, 1};
            offsetMatrix[0][2] = {-1, 1};
            offsetMatrix[0][3] = {-1, 0};
        } else {
            offsetMatrix[0][0] = {0, 0};
            offsetMatrix[0][1] = {0, 0};
            offsetMatrix[0][2] = {0, 0};
            offsetMatrix[0][3] = {0, 0};

            offsetMatrix[1][0] = {0, 0};
            offsetMatrix[1][1] = {1, 0};
            offsetMatrix[1][2] ={0, 0};
            offsetMatrix[1][3] = {-1, 0};

            offsetMatrix[2][0] = {0, 0};
            offsetMatrix[2][1] = {1, 1};
            offsetMatrix[2][2] = {0, 0};
            offsetMatrix[2][3] = {-1, 1};

            offsetMatrix[3][0] = {0, 0};
            offsetMatrix[3][1] = {0, -2};
            offsetMatrix[3][2] = {0, 0};
            offsetMatrix[3][3] = {0, -2};

            offsetMatrix[4][0] = {0, 0};
            offsetMatrix[4][1] = {1, -2};
            offsetMatrix[4][2] = {0, 0};
            offsetMatrix[4][3] = {-1, -2};
        }
}
    /*!
     * 4 - tiles count
     * 5 - offsets presets count
     */
    ::std::array<::std::array<Vector2i, 4>, 5> offsetMatrix{0};
//     ::std::map<PieceComponent::Shape, ::std::array<::std::array<Vector2i, 4>, 5>> offsetMatrix;
};