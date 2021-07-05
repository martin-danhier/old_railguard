#pragma once

#include "../core/WindowManager.h"
#include "../core/Match.h"

namespace railguard::rendering
{
    // Use a typedef to specify which type will be used for swapchain ids
    // That way, if we need to change that type, we only need to do it here
    typedef uint32_t swapchain_id_t;

    class SwapchainManager
    {
    private:

        std::unordered_map<swapchain_id_t, size_t> _idLookupMap;
        std::vector<swapchain_id_t> _id;
        std::vector<vk::SwapchainKHR> _swapchain;
        std::vector<vk::Format> _swapchainImageFormat;
        // std::vector<vk::Format> _depthImageFormat;
        std::vector<std::vector<vk::Image>> _swapchainImages;
        std::vector<std::vector<vk::ImageView>> _swapchainImageViews;

        swapchain_id_t _lastUsedId = 0;

    public:
        explicit SwapchainManager(size_t defaultCapacity = 1);

        /**
         * @brief Create a swapchain that will be used to render on a window.
         *
         * @return swapchain_id_t The id of the new swapchain
         */
        swapchain_id_t CreateWindowSwapchain(const vk::Device &device, const vk::PhysicalDevice &physicalDevice, const vk::SurfaceKHR &surface, const core::WindowManager &windowManager);
    
        [[nodiscard]] core::Match LookupId(swapchain_id_t id);

        void DestroySwapchain(const vk::Device &device, const core::Match &match);

        /**
         * @brief Destroy and recreate the given swapchain at the same slot. Invalidates vulkan objects that used this swapchain.
         *
         * @param match Match mapping the swapchain id to its slot (returned by Lookup function)
         * @param device Vulkan device
         * @param physicalDevice Vulkan physical device
         * @param surface Vulkan surface
         * @param windowManager Window manager
         */
        void RecreateWindowSwapchain(const core::Match &match, const vk::Device &device, const vk::PhysicalDevice &physicalDevice, const vk::SurfaceKHR &surface, const core::WindowManager &windowManager);

        // Getters

        [[nodiscard]] vk::SwapchainKHR GetSwapchain(const core::Match &match) const;
        [[nodiscard]] vk::Format GetSwapchainImageFormat(const core::Match &match) const;
        [[nodiscard]] std::vector<vk::Image> GetSwapchainImages(const core::Match &match) const;
        [[nodiscard]] std::vector<vk::ImageView> GetSwapchainImageViews(const core::Match &match) const;
    };
}