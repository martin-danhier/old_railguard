#pragma once

#include "../core/WindowManager.h"
#include "./SwapchainCameraManager.h"
#include "init/VulkanInit.h"

namespace railguard::rendering
{

    class Renderer
    {
    private:
        // Vulkan global objects
        vk::Instance _instance = nullptr;
        vk::DebugUtilsMessengerEXT _debugMessenger = nullptr;
        vk::SurfaceKHR _surface = nullptr;
        vk::PhysicalDevice _physicalDevice = nullptr;
        vk::PhysicalDeviceProperties _physicalDeviceProperties = {};
        vk::Device _device = nullptr;
        vk::Queue _graphicsQueue = nullptr;
        uint32_t _graphicsQueueFamily = 0;
        VmaAllocator _allocator = nullptr;

        // Other internal variables
        swapchain_id_t _mainWindowSwapchain = 0;

        // Various managers for core objects
        SwapchainManager _swapchainManager;
        SwapchainCameraManager _swapchainCameraManager;

    public:
        explicit Renderer(const core::WindowManager &windowManager);

        /**
         * @brief Renders an image for every camera
         */
        void Draw();

        ~Renderer();
    };

} // namespace railguard::rendering
