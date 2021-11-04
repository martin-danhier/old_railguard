#pragma once

#include "EventSender.h"

#define VK_DEFINE_HANDLE(object) typedef struct object##_T* object;
VK_DEFINE_HANDLE(VkSurfaceKHR);
VK_DEFINE_HANDLE(VkInstance);
struct VkExtent2D;

// Forward declarations
struct SDL_Window;

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
        EventSender<VkExtent2D> _resizeEvent;

    public:
        WindowManager(int defaultWidth, int defaultHeight, std::string defaultTitle);
        static void HandleError();
        void CreateWindow();
        void DestroyWindow();
        [[nodiscard]] static uint64_t GetPerformanceCounter();
        [[nodiscard]] static uint64_t GetPerformanceFrequency();
        [[nodiscard]] std::vector<const char*> GetRequiredVulkanExtensions() const;
        [[nodiscard]] VkSurfaceKHR GetVulkanSurface(VkInstance instance) const;
        [[nodiscard]] VkExtent2D GetWindowExtent() const;
        bool HandleEvents();

        [[nodiscard]] EventSender<VkExtent2D> &ResizeEvent();
    };
} // namespace railguard::core
