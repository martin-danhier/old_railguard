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

        // Device handle to avoid requiring it everywhere
        vk::Device _vulkanDevice = nullptr;
        vk::PhysicalDevice _vulkanPhysicalDevice= nullptr;

        std::unordered_map<swapchain_id_t, size_t> _idLookupMap;
        std::vector<swapchain_id_t> _ids;
        std::vector<vk::SwapchainKHR> _swapchains;
        std::vector<vk::Format> _swapchainImageFormats;
        // std::vector<vk::Format> _depthImageFormat;
        std::vector<std::vector<vk::Image>> _swapchainsImages;
        std::vector<std::vector<vk::ImageView>> _swapchainsImageViews;
        std::vector<std::vector<vk::Framebuffer>> _frameBuffers;

        swapchain_id_t _lastUsedId = 0;

#ifndef NDEBUG
        // In debug mode, keep track of the init status to ensure that Init is called first
        bool _initialized = false;
#endif

    public:
        void Init(const vk::Device &device, const vk::PhysicalDevice &physicalDevice, size_t defaultCapacity = 1);

        /**
         * @brief Destroys every remaining swapchain.
         */
        void Clear();

        /**
         * @brief Create a swapchain that will be used to render on a window.
         *
         * @return swapchain_id_t The id of the new swapchain
         */
        swapchain_id_t CreateWindowSwapchain(const vk::SurfaceKHR &surface, const core::WindowManager &windowManager, const vk::RenderPass &renderPass);

        [[nodiscard]] core::Match LookupId(swapchain_id_t id);

        void DestroySwapchain(const core::Match &match);

        /**
         * @brief Destroy and recreate the given swapchain at the same slot. Invalidates vulkan objects that used this swapchain.
         *
         * @param match Match mapping the swapchain id to its slot (returned by Lookup function)
         * @param device Vulkan device
         * @param physicalDevice Vulkan physical device
         * @param surface Vulkan surface
         * @param windowManager Window manager
         */
        void RecreateWindowSwapchain(const core::Match &match, const vk::SurfaceKHR &surface, const core::WindowManager &windowManager, const vk::RenderPass &renderPass);

        // Getters

        [[nodiscard]] vk::SwapchainKHR GetSwapchain(const core::Match &match) const;
        [[nodiscard]] vk::Format GetSwapchainImageFormat(const core::Match &match) const;
        [[nodiscard]] std::vector<vk::Image> GetSwapchainImages(const core::Match &match) const;
        [[nodiscard]] std::vector<vk::ImageView> GetSwapchainImageViews(const core::Match &match) const;
        [[nodiscard]] std::vector<vk::Framebuffer> GetFramebuffers(const core::Match &match) const;
    };
}