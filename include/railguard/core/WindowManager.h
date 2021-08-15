#pragma once

#include "EventSender.h"

// Forward declarations
struct SDL_Window;

namespace vk {
    struct Extent2D;
    class SurfaceKHR;
    class Instance;
}

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
        std::string _title;
        
        // Event thrown when the window is resized
        EventSender<vk::Extent2D> _resizeEvent;

    public:
        WindowManager(int defaultWidth, int defaultHeight, std::string defaultTitle);
        static void HandleError();
        void CreateWindow();
        void DestroyWindow();
        [[nodiscard]] static uint64_t GetPerformanceCounter();
        [[nodiscard]] static uint64_t GetPerformanceFrequency();
        [[nodiscard]] std::vector<const char*> GetRequiredVulkanExtensions() const;
        [[nodiscard]] vk::SurfaceKHR GetVulkanSurface(vk::Instance instance) const;
        [[nodiscard]] vk::Extent2D GetWindowExtent() const;
        bool HandleEvents();

        [[nodiscard]] EventSender<vk::Extent2D> &ResizeEvent();
    };
} // namespace railguard::core
