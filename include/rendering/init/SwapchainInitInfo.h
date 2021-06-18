#pragma once

#include "VkInitIncludes.h"

namespace railguard::rendering::init
{
    /**
     * @brief Contains pointers to the destination variable of vulkan handles.
     * They will be filled when VulkanInit::InitSwapchain() is called.
     * The struct also contains references to other vulkan handles that must be set previously.
     */
    struct SwapchainInitInfo
    {
        /**
         * @brief Readonly reference to the vulkan device
         */
        const vk::Device &device;
        /**
         * @brief Readonly reference to the vulkan physical device
         */
        const vk::PhysicalDevice &physicalDevice;
        /**
         * @brief Readonly reference to the surface
         */
        const vk::SurfaceKHR &surface;
        /**
         * @brief Readonly reference to the window manager
         */
        const railguard::core::WindowManager &windowManager;
        /**
         * @brief Pointer to the destination variable for the swapchain
         */
        vk::SwapchainKHR *swapchain;
        /**
         * @brief Pointer to the destination variable of the swapchain images
         */
        std::vector<vk::Image> *swapchainImages;
        /**
         * @brief Pointer to the destination variable of the swapchain image format
         */
        vk::Format *swapchainImageFormat;
        /**
         * @brief Pointer to the destination variable of the swapchain image views
         */
        std::vector<vk::ImageView> *swapchainImageViews;
    };

}