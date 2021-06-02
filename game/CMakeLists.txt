cmake_minimum_required(VERSION 3.17)

add_definitions(-DDEBUG)

add_executable(${PROJECT_NAME}
        src/main.cpp
        )

if (APPLE)
    target_compile_options(${PROJECT_NAME} PRIVATE "-fobjc-arc")
elseif (WIN32)
    # none
endif()

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

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets)
