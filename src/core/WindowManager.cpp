#include "railguard/core/WindowManager.h"

#include "railguard/includes/Vulkan.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>
#include <iostream>
#include <utility>

namespace railguard::core
{
    WindowManager::WindowManager(int defaultWidth, int defaultHeight, std::string defaultTitle)
        : _width(defaultWidth),
          _height(defaultHeight),
          _title(std::move(defaultTitle))
    {
        CreateWindow();
    }

    void WindowManager::HandleError()
    {
        std::cerr << "[SDL Error]\n" << SDL_GetError() << '\n';
    }

    void WindowManager::CreateWindow()
    {
        // Create a window
        SDL_WindowFlags windowFlags = SDL_WINDOW_VULKAN;

        _window = SDL_CreateWindow(_title.c_str(),
                                   SDL_WINDOWPOS_UNDEFINED,
                                   SDL_WINDOWPOS_UNDEFINED,
                                   static_cast<int32_t>(_width),
                                   static_cast<int32_t>(_height),
                                   windowFlags);
        SDL_SetWindowResizable(_window, SDL_TRUE);

        if (_window == nullptr)
        {
            HandleError();
        }
    }

    void WindowManager::DestroyWindow()
    {
        // Destroy the window
        SDL_DestroyWindow(_window);
    }

    uint64_t WindowManager::GetPerformanceCounter()
    {
        return SDL_GetPerformanceCounter();
    }

    uint64_t WindowManager::GetPerformanceFrequency()
    {
        return SDL_GetPerformanceFrequency();
    }

    std::vector<const char *> WindowManager::GetRequiredVulkanExtensions() const
    {
        // Get the number of required extensions
        uint32_t sdlRequiredExtensionsCount = 0;
        if (!SDL_Vulkan_GetInstanceExtensions(_window, &sdlRequiredExtensionsCount, nullptr))
            core::WindowManager::HandleError();

        // Create a vector with that number and fetch said extensions
        std::vector<const char *> sdlRequiredExtensions(sdlRequiredExtensionsCount);
        if (!SDL_Vulkan_GetInstanceExtensions(_window, &sdlRequiredExtensionsCount, sdlRequiredExtensions.data()))
            core::WindowManager::HandleError();

        // Return the vector
        return sdlRequiredExtensions;
    }

    vk::SurfaceKHR WindowManager::GetVulkanSurface(vk::Instance instance) const
    {
        VkSurfaceKHR surface;
        // Generate surface
        if (!SDL_Vulkan_CreateSurface(_window, instance, &surface))
            HandleError();
        return vk::SurfaceKHR(surface);
    }

    bool WindowManager::HandleEvents()
    {
        SDL_Event event;
        bool shouldQuit = false;

        // Handle all events in a queue
        while (SDL_PollEvent(&event) != 0)
        {
            if (event.type == SDL_QUIT)
            {
                shouldQuit = true;
            }
            // Window event
            else if (event.type == SDL_WINDOWEVENT)
            {
                if (event.window.event == SDL_WINDOWEVENT_RESIZED)
                {
                    _resizeEvent.SendEvent({static_cast<uint32_t>(event.window.data1), static_cast<uint32_t>(event.window.data2)});
                }
            }
            // ...
            // Other types here
        }

        return shouldQuit;
    }

    EventSender<vk::Extent2D> &WindowManager::ResizeEvent()
    {
        return _resizeEvent;
    }

    vk::Extent2D WindowManager::GetWindowExtent() const
    {
        return vk::Extent2D {
            .width  = _width,
            .height = _height,
        };
    }

} // namespace railguard::core
