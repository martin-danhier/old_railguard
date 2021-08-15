#include <iostream>
#include <railguard/core/Engine.h>
#include <railguard/core/EntityManager.h>
#include <railguard/core/WindowManager.h>
#include <railguard/rendering/Renderer.h>
#include <railguard/rendering/SwapchainCameraManager.h>
#include <railguard/utils/Colors.h>
#include <railguard/rendering/init/CameraInitInfo.h>

#define DEFAULT_ENTITY_MANAGER_CAPACITY 1000

namespace railguard::core
{
    Engine::Engine()
    {
        _entityManager = new EntityManager(DEFAULT_ENTITY_MANAGER_CAPACITY);
        _windowManager = new WindowManager(500, 500, "Railguard");
        _renderer      = new rendering::Renderer(*_windowManager);

        // Create camera
        auto mainCamera = _entityManager->CreateEntity();
        auto camManager = _renderer->GetSwapchainCameraManager();

        camManager->CreateComponent(
            mainCamera, rendering::init::SwapchainCameraInitInfo {.swapchainId = 1, .clearColor = utils::GetColorHex(0xff0000ff)});
        std::cout << "Engine initialized successfully.\n";
    }

    Engine::~Engine()
    {
        _windowManager->DestroyWindow();

        // Cleanup
        delete _renderer;
        delete _windowManager;
        delete _entityManager;

        std::cout << "Engine destructed successfully.\n";
    }

    void Engine::RunMainLoop()
    {
        // Init variables
        bool shouldQuit           = false;
        uint64_t currentFrameTime = 0;

        // Main loop
        while (!shouldQuit)
        {
            // Update deltatime
            uint64_t previousFrameTime = currentFrameTime;
            currentFrameTime           = WindowManager::GetPerformanceCounter();
            _deltaTime                 = static_cast<double_t>(currentFrameTime - previousFrameTime)
                         / static_cast<double_t>(WindowManager::GetPerformanceFrequency());

            // Handle window events
            shouldQuit = _windowManager->HandleEvents();

            // Render objects
            _renderer->Draw();
        }
    }

} // namespace railguard::core
