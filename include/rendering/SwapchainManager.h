#pragma once

#include "../core/WindowManager.h"
#include "../core/Match.h"
#include "../core/StandaloneManager.h"
#include "./structs/Storages.h"

namespace railguard::rendering
{
    // Use a typedef to specify which type will be used for swapchain ids
    // That way, if we need to change that type, we only need to do it here
    typedef uint32_t swapchain_id_t;

    class SwapchainManager : public core::StandaloneManager<swapchain_id_t, structs::FullDeviceStorage>
    {
    private:
        // Typedef the parent type to make it easier to call from the methods
        typedef core::StandaloneManager<swapchain_id_t, structs::FullDeviceStorage> super;

        std::vector<vk::SwapchainKHR> _swapchains;
        std::vector<vk::Format> _swapchainImageFormats;
        // std::vector<vk::Format> _depthImageFormat;
        std::vector<std::vector<vk::Image>> _swapchainsImages;
        std::vector<std::vector<vk::ImageView>> _swapchainsImageViews;
        std::vector<std::vector<vk::Framebuffer>> _frameBuffers;

    public:
        void Init(structs::FullDeviceStorage storage, size_t defaultCapacity = 1);

        /**
         * @brief Destroys every remaining swapchain.
         */
        void Clear();

        /**
         * @brief Create a swapchain that will be used to render on a window.
         *
         * @return swapchain_id_t The id of the new swapchain
         */
        core::CompleteMatch<swapchain_id_t> CreateWindowSwapchain(const vk::SurfaceKHR &surface, const core::WindowManager &windowManager, const vk::RenderPass &renderPass);

        /**
         * @brief Destroys the swapchain pointed by the given match
         *
         * @param match match representing the position of the swapchain
         */
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