#pragma once

#include "railguard/core/StandaloneManager.h"
#include "railguard/includes/Vulkan.h"
#include "railguard/rendering/Ids.h"

namespace railguard
{
    namespace core
    {
        class Match;
        class WindowManager;
    } // namespace core

    namespace rendering
    {
        struct SwapchainManagerStorage
        {
          public:
            vk::Device vulkanDevice                 = nullptr;
            vk::PhysicalDevice vulkanPhysicalDevice = nullptr;
            const class FrameManager *frameManager  = nullptr;
        };

        class SwapchainManager : public core::StandaloneManager<swapchain_id_t, SwapchainManagerStorage>
        {
          private:
            // Typedef the parent type to make it easier to call from the methods
            typedef core::StandaloneManager<swapchain_id_t, SwapchainManagerStorage> super;

            std::vector<vk::SwapchainKHR> _swapchains;
            std::vector<vk::Format> _swapchainImageFormats;
            std::vector<vk::Extent2D> _viewportExtents;
            // std::vector<vk::Format> _depthImageFormat;
            std::vector<std::vector<vk::Image>> _swapchainsImages;
            std::vector<std::vector<vk::ImageView>> _swapchainsImageViews;
            std::vector<std::vector<vk::Framebuffer>> _frameBuffers;
            std::vector<uint64_t> _lastTimeSubmitted;
            std::vector<uint32_t> _imageIndex;

            void CleanUp();

          public:
            SwapchainManager(SwapchainManagerStorage storage, size_t defaultCapacity);
            ~SwapchainManager();
            /**
             * @brief Destroys every remaining swapchain.
             */
            void Clear() override;

            /**
             * @brief Create a swapchain that will be used to render on a window.
             *
             * @return swapchain_id_t The id of the new swapchain
             */
            [[nodiscard]] core::CompleteMatch<swapchain_id_t> CreateWindowSwapchain(const vk::SurfaceKHR &surface,
                                                                                    const core::WindowManager &windowManager,
                                                                                    const vk::RenderPass &renderPass);

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
            void RecreateWindowSwapchain(const core::Match &match,
                                         const vk::SurfaceKHR &surface,
                                         const vk::Extent2D &newExtent,
                                         const vk::RenderPass &renderPass);

            /**
             * @brief Requests the next image of the given swapchain.
             *
             * @param match Match mapping the swapchain id to its slot.
             *
             * @throws runtime_error If the image could not be acquired.
             */
            [[nodiscard]] uint32_t RequestNextImageIndex(const core::Match &match);

            void PresentImage(const core::Match &match, const vk::Queue &graphicsQueue);
            void PresentImage(size_t index, const vk::Queue &graphicsQueue);
            void PresentUsedImages(const vk::Queue &graphicsQueue);

            // Getters

            [[nodiscard]] vk::SwapchainKHR GetSwapchain(const core::Match &match) const;
            [[nodiscard]] vk::Format GetSwapchainImageFormat(const core::Match &match) const;
            [[nodiscard]] std::vector<vk::Image> GetSwapchainImages(const core::Match &match) const;
            [[nodiscard]] std::vector<vk::ImageView> GetSwapchainImageViews(const core::Match &match) const;
            [[nodiscard]] std::vector<vk::Framebuffer> GetFramebuffers(const core::Match &match) const;
            [[nodiscard]] vk::Extent2D GetViewportExtent(const core::Match &match) const;
        };
    } // namespace rendering
} // namespace railguard