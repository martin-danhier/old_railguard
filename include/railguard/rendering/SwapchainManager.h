#pragma once

#include <railguard/core/StandaloneManager.h>
#include <railguard/rendering/Ids.h>

#define VK_DEFINE_HANDLE(object) typedef struct object##_T* object;
typedef uint32_t VkFormat;
struct VkExtent2D;
VK_DEFINE_HANDLE(VkDevice);
VK_DEFINE_HANDLE(VkPhysicalDevice);
VK_DEFINE_HANDLE(VkSwapchainKHR);
VK_DEFINE_HANDLE(VkImage)
VK_DEFINE_HANDLE(VkImageView);
VK_DEFINE_HANDLE(VkFramebuffer);
VK_DEFINE_HANDLE(VkRenderPass);
VK_DEFINE_HANDLE(VkQueue);
VK_DEFINE_HANDLE(VkSurfaceKHR);

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
            VkDevice vulkanDevice                 = nullptr;
            VkPhysicalDevice vulkanPhysicalDevice = nullptr;
            const class FrameManager *frameManager  = nullptr;
        };

        class SwapchainManager : public core::StandaloneManager<swapchain_id_t, SwapchainManagerStorage>
        {
          private:
            // Typedef the parent type to make it easier to call from the methods
            typedef core::StandaloneManager<swapchain_id_t, SwapchainManagerStorage> super;

            std::vector<VkSwapchainKHR> _swapchains;
            std::vector<VkFormat> _swapchainImageFormats;
            std::vector<VkExtent2D> _viewportExtents;
            // std::vector<VkFormat> _depthImageFormat;
            std::vector<std::vector<VkImage>> _swapchainsImages;
            std::vector<std::vector<VkImageView>> _swapchainsImageViews;
            std::vector<std::vector<VkFramebuffer>> _frameBuffers;
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
            [[nodiscard]] core::CompleteMatch<swapchain_id_t> CreateWindowSwapchain(const VkSurfaceKHR &surface,
                                                                                    const core::WindowManager &windowManager,
                                                                                    const VkRenderPass &renderPass);

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
                                         const VkSurfaceKHR &surface,
                                         const VkExtent2D &newExtent,
                                         const VkRenderPass &renderPass);

            /**
             * @brief Requests the next image of the given swapchain.
             *
             * @param match Match mapping the swapchain id to its slot.
             *
             * @throws runtime_error If the image could not be acquired.
             */
            [[nodiscard]] uint32_t RequestNextImageIndex(const core::Match &match);

            void PresentImage(const core::Match &match, const VkQueue &graphicsQueue);
            void PresentImage(size_t index, const VkQueue &graphicsQueue);
            void PresentUsedImages(const VkQueue &graphicsQueue);

            // Getters

            [[nodiscard]] VkSwapchainKHR GetSwapchain(const core::Match &match) const;
            [[nodiscard]] VkFormat GetSwapchainImageFormat(const core::Match &match) const;
            [[nodiscard]] std::vector<VkImage> GetSwapchainImages(const core::Match &match) const;
            [[nodiscard]] std::vector<VkImageView> GetSwapchainImageViews(const core::Match &match) const;
            [[nodiscard]] std::vector<VkFramebuffer> GetFramebuffers(const core::Match &match) const;
            [[nodiscard]] VkExtent2D GetViewportExtent(const core::Match &match) const;
        };
    } // namespace rendering
} // namespace railguard