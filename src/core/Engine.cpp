#include "core/Engine.h"
#include <iostream>
#include <functional>

#define DEFAULT_ENTITY_MANAGER_CAPACITY 1000

namespace railguard::core
{
    Engine::Engine() : _entityManager(DEFAULT_ENTITY_MANAGER_CAPACITY),
                       _windowManager(500, 500, "Railguard"),
                       _deltaTime{0}
    {
        _windowManager.ResizeEvent().RegisterListener("test", std::bind(&Engine::OnWindowResized, this, std::placeholders::_1));
        
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

            // Call components
            
        }
    }

    void Engine::OnWindowResized(const Extent2D &newSize) {
        std::cout << "New size: " << newSize.width << ", " << newSize.height << '\n';
    }

} // namespace core
