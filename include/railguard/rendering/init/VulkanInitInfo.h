#pragma once

namespace railguard
{
    namespace core
    {
        class WindowManager;
    }

    namespace rendering::init
    {
        /**
         * @brief Contains pointers to the destination variable of vulkan handles.
         * They will be filled when VulkanInit::InitVulkan() is called.
         */
        struct VulkanInitInfo
        {
            /**
             * @brief Reference to the window manager
             */
            const core::WindowManager &windowManager;
            /**
             * @brief Pointer to a variable that will hold the vulkan instance
             */
            vk::Instance *instance = nullptr;
            /**
             * @brief Pointer to a variable that will hold the vulkan debug messenger
             */
            vk::DebugUtilsMessengerEXT *debugMessenger = nullptr;
            /**
             * @brief Pointer to a variable that will hold the vulkan surface
             */
            vk::SurfaceKHR *surface = nullptr;
            /**
             * @brief Pointer to a variable that will hold the physical device
             */
            vk::PhysicalDevice *physicalDevice = nullptr;
            /**
             * @brief Pointer to a variable that will hold the physical device properties
             */
            vk::PhysicalDeviceProperties *physicalDeviceProperties = nullptr;
            /**
             * @brief Pointer to a variable that will hold the vulkan device
             */
            vk::Device *device = nullptr;
            /**
             * @brief Pointer to a variable that will hold the vulkan device
             */
            vk::Queue *graphicsQueue = nullptr;
            /**
             * @brief Pointer to a variable that will hold the vulkan device
             */
            uint32_t *graphicsQueueFamily = nullptr;
        };
    } // namespace rendering::init
} // namespace railguard