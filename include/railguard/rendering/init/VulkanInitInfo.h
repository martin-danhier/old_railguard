#pragma once

#include <cstdint>

#define VK_DEFINE_HANDLE(object) typedef struct object##_T* object;
VK_DEFINE_HANDLE(VkInstance)
VK_DEFINE_HANDLE(VkDebugUtilsMessengerEXT)
VK_DEFINE_HANDLE(VkSurfaceKHR)
VK_DEFINE_HANDLE(VkPhysicalDevice)
VK_DEFINE_HANDLE(VkPhysicalDeviceProperties)
VK_DEFINE_HANDLE(VkDevice)
VK_DEFINE_HANDLE(VkQueue)


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
            VkInstance *instance = nullptr;
            /**
             * @brief Pointer to a variable that will hold the vulkan debug messenger
             */
            VkDebugUtilsMessengerEXT *debugMessenger = nullptr;
            /**
             * @brief Pointer to a variable that will hold the vulkan surface
             */
            VkSurfaceKHR *surface = nullptr;
            /**
             * @brief Pointer to a variable that will hold the physical device
             */
            VkPhysicalDevice *physicalDevice = nullptr;
            /**
             * @brief Pointer to a variable that will hold the physical device properties
             */
            VkPhysicalDeviceProperties *physicalDeviceProperties = nullptr;
            /**
             * @brief Pointer to a variable that will hold the vulkan device
             */
            VkDevice *device = nullptr;
            /**
             * @brief Pointer to a variable that will hold the vulkan device
             */
            VkQueue *graphicsQueue = nullptr;
            /**
             * @brief Pointer to a variable that will hold the vulkan device
             */
            uint32_t *graphicsQueueFamily = nullptr;
        };
    } // namespace rendering::init
} // namespace railguard