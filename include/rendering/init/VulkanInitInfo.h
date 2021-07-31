#pragma once

#include "VkInitIncludes.h"

namespace railguard::rendering::init
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
        vk::Instance *instance;
        /**
         * @brief Pointer to a variable that will hold the vulkan debug messenger
         */
        vk::DebugUtilsMessengerEXT *debugMessenger;
        /**
         * @brief Pointer to a variable that will hold the vulkan surface
         */
        vk::SurfaceKHR *surface;
        /**
         * @brief Pointer to a variable that will hold the physical device
         */
        vk::PhysicalDevice *physicalDevice;
        /**
         * @brief Pointer to a variable that will hold the physical device properties
         */
        vk::PhysicalDeviceProperties *physicalDeviceProperties;
        /**
         * @brief Pointer to a variable that will hold the vulkan device
         */
        vk::Device *device;
        /**
         * @brief Pointer to a variable that will hold the vulkan device
         */
        vk::Queue *graphicsQueue;
        /**
         * @brief Pointer to a variable that will hold the vulkan device
         */
        uint32_t *graphicsQueueFamily;
        /**
         * @brief Pointer to a variable that will hold the VMA allocator
         */
        VmaAllocator *allocator;

    };
}
