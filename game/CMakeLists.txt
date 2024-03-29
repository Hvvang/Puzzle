cmake_minimum_required(VERSION 3.17)

add_definitions(-DDEBUG)

set(Puzzle_SOURCES
        src/App.cpp
        src/Settings.cpp

        src/Systems/AnimationSystem.cpp
        src/Systems/GridController.cpp
        src/Systems/TileController.cpp
        src/Systems/PieceController.cpp
        src/Systems/CollisionSystem.cpp
        src/Systems/ScoreSystem.cpp

        src/MenuController.cpp
        src/GameController.cpp

        src/Font/Font.cpp
        src/Font/Text.cpp

        )

set(Puzzle_HEADERS
        include/App.hpp
        include/Settings.hpp

        include/Components/AnimationComponent.hpp
        include/Components/PieceComponent.hpp
        include/Components/ScoreComponent.hpp
        include/Components/TileComponent.hpp
        include/Components/BoardComponent.hpp
        include/Components/MoveComponent.hpp
        include/Components/PieceCollisionComponent.hpp

        include/Systems/AnimationSystem.hpp
        include/Systems/GridController.hpp
        include/Systems/PieceController.hpp
        include/Systems/TileController.hpp
        include/Systems/CollisionSystem.hpp
        include/Systems/ScoreSystem.hpp

        include/GameController.hpp
        include/MenuController.hpp

        include/Event/Event.hpp
        include/Event/BaseHandlerFunction.hpp
        include/Event/HandlerFunction.hpp
        include/Event/EventSystem.hpp
        include/Event/GameEvents.hpp

        include/Font/Glyph.hpp
        include/Font/Font.hpp
        include/Font/Text.hpp
        include/Font/TextRenderer.hpp

        include/Utils/Button.hpp

        )


add_executable(${PROJECT_NAME}
        main.cpp
        ${Puzzle_SOURCES}

        ${Puzzle_HEADERS}
        )

if (APPLE)
    target_compile_options(${PROJECT_NAME} PRIVATE "-fobjc-arc")
elseif (WIN32)
    # none
endif()

target_include_directories(${PROJECT_NAME}
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        PRIVATE
        src
        )

target_link_libraries(${PROJECT_NAME}
        PRIVATE
        MiniKit::MiniKit
        Engine::Utils
        Engine::Math
        Engine::Loader
        Engine::ECS
        Engine::Input
        Engine::View

        nlohmann_json::nlohmann_json
        )

#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/..")
#set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory
                          ${CMAKE_SOURCE_DIR}/game/assets $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets)
