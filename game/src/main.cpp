#include <MiniKit/MiniKit.hpp>

#include <Math.hpp>

#include <InputManager.hpp>
#include <EntityManager.hpp>
#include <SpriteLoader.hpp>
#include <Camera.hpp>

#include <Components/Sprite.hpp>

#include <Systems/SpriteRenderingSystem.hpp>

using ::Engine::Math::Vector2f;

using ::Engine::Math::operator*;

class App final : public ::MiniKit::Engine::Application {
public:
    ::std::error_code Start(::MiniKit::Engine::Context& context) noexcept override {

    }

    ::std::error_code Shutdown(::MiniKit::Engine::Context& context) noexcept override {
        return ::MiniKit::MakeErrorCode(::MiniKit::Error::None);
    }

    void Tick(::MiniKit::Engine::Context& context) noexcept override {

    }

private:

};

int main() {
    App app{};

    ::MiniKit::Engine::StartupInfo startupInfo{};

    startupInfo.window.title = "Puzzle";
    startupInfo.window.width = 1366;
    startupInfo.window.height = 768;
    startupInfo.enableVSync = true;

    return ::MiniKit::ApplicationMain(startupInfo, app);
}
