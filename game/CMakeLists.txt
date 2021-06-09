cmake_minimum_required(VERSION 3.17)

add_definitions(-DDEBUG)

set(Puzzle_SOURCES
        src/App.cpp

        src/Systems/BoardController.cpp
        src/Systems/TileController.cpp
        src/Systems/PieceController.cpp

        )

set(Puzzle_HEADERS
        include/App.hpp

        include/Components/PieceComponent.hpp

        include/Components/TileComponent.hpp
        include/Components/BoardComponent.hpp

        include/Systems/BoardController.hpp
        include/Systems/PieceController.hpp
        include/Systems/TileController.hpp

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
        )

#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/..")
#set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory
                          ${CMAKE_SOURCE_DIR}/game/assets $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets)
