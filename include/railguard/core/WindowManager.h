#pragma once

#include "SDL2/SDL.h"
#include "EventSender.h"
#include <string>

#include "railguard/includes/Vulkan.h"

namespace railguard::core
{

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
        EventSender<vk::Extent2D> _resizeEvent;

    public:
        WindowManager(int defaultWidth, int defaultHeight, const std::string &defaultTitle);
        static void HandleError();
        void CreateWindow();
        void DestroyWindow();
        [[nodiscard]] uint64_t GetPerformanceCounter();
        [[nodiscard]] uint64_t GetPerformanceFrequency();
        [[nodiscard]] std::vector<const char*> GetRequiredVulkanExtensions() const;
        [[nodiscard]] vk::SurfaceKHR GetVulkanSurface(vk::Instance instance) const;
        [[nodiscard]] vk::Extent2D GetWindowExtent() const;
        bool HandleEvents();

        [[nodiscard]] EventSender<vk::Extent2D> &ResizeEvent();
    };
} // namespace railguard::core
