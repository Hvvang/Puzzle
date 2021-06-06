#pragma once

#include <MiniKit/MiniKit.hpp>

class App final : public ::MiniKit::Engine::Application {
public:
    ::std::error_code Start(::MiniKit::Engine::Context& context) noexcept override;

    ::std::error_code Shutdown(::MiniKit::Engine::Context& context) noexcept override;

    void Tick(::MiniKit::Engine::Context& context) noexcept override;

private:

};


