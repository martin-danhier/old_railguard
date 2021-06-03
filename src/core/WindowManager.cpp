#include "core/WindowManager.h"
#include <iostream>

namespace railguard::core
{

    WindowManager::WindowManager(int defaultWidth, int defaultHeight, const std::string &defaultTitle)
        : _width(defaultWidth),
          _height(defaultHeight),
          _title(defaultTitle),
          _resizeEvent(1)
    {
        CreateWindow();
    }

    void WindowManager::CreateWindow()
    {
        // Create a window
        SDL_WindowFlags windowFlags = SDL_WINDOW_VULKAN;
        _window = SDL_CreateWindow(_title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, _width, _height, windowFlags);

        if (_window == nullptr) {
            // Error
            std::cerr << "Error creating window\n";
        }
    }

    void WindowManager::DestroyWindow()
    {
        // Destroy the window
        SDL_DestroyWindow(_window);
    }

    uint64_t WindowManager::GetPerformanceCounter() {
        return SDL_GetPerformanceCounter();
    }

    uint64_t WindowManager::GetPerformanceFrequency() {
        return SDL_GetPerformanceFrequency();
    }

    bool WindowManager::HandleEvents() {
        SDL_Event event;
        bool shouldQuit = false;

        // Handle all events in a queue
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT){
                shouldQuit = true;
            }
            // ...
            // Other types here
        }

        return shouldQuit;
    }

} // namespace core
