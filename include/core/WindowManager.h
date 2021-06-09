#pragma once

#include "SDL2/SDL.h"
#include "EventSender.h"
#include <string>

// We want to dynamically load Vulkan functions
#define VULKAN_HPP_DISPATCH_LOADER_DYNAMIC 1
#include <vulkan/vulkan.hpp>

namespace railguard::core
{
    struct Extent2D
    {
        uint32_t width;
        uint32_t height;
    };

    class WindowManager
    {
    private:
        /**
         * @brief Handle for the main SDL window
         */
        SDL_Window *_window{nullptr};
        uint32_t _width{0};
        uint32_t _height{0};
        std::string _title{""};
        // Event thrown when the window is resized
        EventSender<Extent2D> _resizeEvent;

    public:
        WindowManager(int defaultWidth, int defaultHeight, const std::string &defaultTitle);
        static void HandleError();
        void CreateWindow();
        void DestroyWindow();
        [[nodiscard]] uint64_t GetPerformanceCounter();
        [[nodiscard]] uint64_t GetPerformanceFrequency();
        [[nodiscard]] std::vector<const char*> GetRequiredVulkanExtensions() const;
        [[nodiscard]] vk::SurfaceKHR GetVulkanSurface(vk::Instance instance) const;
        bool HandleEvents();

        [[nodiscard]] EventSender<Extent2D> &ResizeEvent();
    };
} // namespace railguard::core
