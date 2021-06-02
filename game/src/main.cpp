#include <MiniKit/MiniKit.hpp>

#include <App.hpp>

int main() {
    App app;
    ::MiniKit::Engine::StartupInfo startupInfo{};

    startupInfo.window.title = "Puzzle";
    startupInfo.window.width = 1366;
    startupInfo.window.height = 768;
    startupInfo.enableVSync = true;

    return ::MiniKit::ApplicationMain(startupInfo, app);
}
