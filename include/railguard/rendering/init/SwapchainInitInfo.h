#pragma once

#include <vector>

#include <vulkan/vulkan.h> // Can't forward declare damn enum VkFormat

// #define VK_DEFINE_HANDLE(object) typedef struct object##_T* object;
// VK_DEFINE_HANDLE(VkDevice)
// VK_DEFINE_HANDLE(VkPhysicalDevice)
// VK_DEFINE_HANDLE(VkSurfaceKHR)
// VK_DEFINE_HANDLE(VkRenderPass)
// VK_DEFINE_HANDLE(VkSwapchainKHR)
// VK_DEFINE_HANDLE(VkImage)
// VK_DEFINE_HANDLE(VkImageView)
// VK_DEFINE_HANDLE(VkFramebuffer)
// struct VkExtent2D;


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
        const VkDevice &device;
        /**
         * @brief Readonly reference to the vulkan physical device
         */
        const VkPhysicalDevice &physicalDevice;
        /**
         * @brief Readonly reference to the surface
         */
        const VkSurfaceKHR &surface;
        /**
         * @brief Readonly reference to the extent of the window
         */
        const VkExtent2D &windowExtent;
        /**
         * @brief Pointer to the renderpass that should be linked to the framebuffers
         * */
        const VkRenderPass &renderPass;
        /**
         * @brief Pointer to the destination variable for the swapchain
         */
        VkSwapchainKHR *swapchain;
        /**
         * @brief Pointer to the destination variable of the swapchain images
         */
        std::vector<VkImage> *swapchainImages;
        /**
         * @brief Pointer to the destination variable of the swapchain image format
         */
        VkFormat *swapchainImageFormat;
        /**
         * @brief Pointer to the destination variable of the swapchain image views
         */
        std::vector<VkImageView> *swapchainImageViews;
        /**
         * @brief Pointer to the destination variable of the framebuffers
         */
        std::vector<VkFramebuffer> *swapchainFramebuffers;
    };

}