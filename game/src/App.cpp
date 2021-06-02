#include "App.hpp"

::std::error_code App::Start(MiniKit::Engine::Context &context) noexcept {
    return std::error_code();
}

::std::error_code App::Shutdown(MiniKit::Engine::Context &context) noexcept {
    return ::MiniKit::MakeErrorCode(::MiniKit::Error::None);
}

void App::Tick(MiniKit::Engine::Context &context) noexcept {

}
