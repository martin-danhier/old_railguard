#include "../../include/core/Engine.h"
#include <iostream>
#include <functional>

#define DEFAULT_ENTITY_MANAGER_CAPACITY 1000

namespace railguard::core
{
    Engine::Engine() : _entityManager(DEFAULT_ENTITY_MANAGER_CAPACITY),
                       _windowManager(500, 500, "Railguard"),
                       _renderer(_windowManager),
                       _deltaTime{0}
    {
        std::cout << "Engine initialized successfully.\n";
    }

    Engine::~Engine()
    {
        _windowManager.DestroyWindow();
        std::cout << "Engine destructed successfully.\n";
    }

    void Engine::RunMainLoop()
    {
        // Init variables
        bool shouldQuit = false;
        uint64_t currentFrameTime = 0;

        // Main loop
        while (!shouldQuit)
        {
            // Update deltatime
            uint64_t previousFrameTime = currentFrameTime;
            currentFrameTime = _windowManager.GetPerformanceCounter();
            _deltaTime = static_cast<double_t>(currentFrameTime - previousFrameTime) /
                         static_cast<double_t>(_windowManager.GetPerformanceFrequency());

            // Handle window events
            shouldQuit = _windowManager.HandleEvents();

            // Render objects
            _renderer.Draw();
            
        }
    }

} // namespace core
