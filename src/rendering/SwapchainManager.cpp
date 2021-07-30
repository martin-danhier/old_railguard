#include "../../../include/rendering/SwapchainManager.h"
#include "../../../include/rendering/init/VulkanInit.h"
#include "../../include/utils/AdvancedCheck.h"

#ifdef USE_ADVANCED_CHECKS
// Define local error messages
#define NOT_INITIALIZED_ERROR "SwapchainManager should be initialized with Init before calling this method."
#endif

namespace railguard::rendering
{

    void SwapchainManager::Init(structs::FullDeviceStorage storage,  size_t defaultCapacity)
    {
        // Call parent function
        super::Init(storage, defaultCapacity);

        // Init vectors that weren't initialized by parent
        _swapchains.reserve(defaultCapacity);
        _swapchainImageFormats.reserve(defaultCapacity);
        _swapchainsImages.reserve(defaultCapacity);
        _swapchainsImageViews.reserve(defaultCapacity);
        _frameBuffers.reserve(defaultCapacity);
    }

    void SwapchainManager::Clear()
    {
        super::Clear();

        // Destroy the framebuffers
        for (auto framebufferVector : _frameBuffers)
        {
            for (auto framebuffer : framebufferVector)
            {
                _storage.vulkanDevice.destroyFramebuffer(framebuffer);
            }
        }

        // Destroy image views
        for (auto imageViewVector : _swapchainsImageViews)
        {
            for (auto imageView : imageViewVector)
            {
                _storage.vulkanDevice.destroyImageView(imageView);
            }
        }

        // We need to destroy every swapchain
        // No need to do other operations on the vectors, since they will be destroyed anyway
        for (auto swapchain : _swapchains)
        {
            _storage.vulkanDevice.destroySwapchainKHR(swapchain);
        }

        // Clear everything
        _swapchains.clear();
        _swapchainImageFormats.clear();
        _swapchainsImageViews.clear();
        _swapchainsImages.clear();
    }

    core::CompleteMatch<swapchain_id_t> SwapchainManager::CreateWindowSwapchain(const vk::SurfaceKHR &surface, const core::WindowManager &windowManager, const vk::RenderPass &renderPass)
    {
        auto match = super::CreateItem();

        // Create temporary destination variables
        vk::SwapchainKHR newSwapchain;
        vk::Format newSwapchainImageFormat;
        std::vector<vk::Image> newSwapchainImages;
        std::vector<vk::ImageView> newSwapchainImageViews;
        std::vector<vk::Framebuffer> newSwapchainFramebuffers;

        // Create the swapchain
        init::SwapchainInitInfo swapchainInitInfo{
            .device = _storage.vulkanDevice,
            .physicalDevice = _storage.vulkanPhysicalDevice,
            .surface = surface,
            .windowManager = windowManager,
            .renderPass = renderPass,
            .swapchain = &newSwapchain,
            .swapchainImages = &newSwapchainImages,
            .swapchainImageFormat = &newSwapchainImageFormat,
            .swapchainImageViews = &newSwapchainImageViews,
            .swapchainFramebuffers = &newSwapchainFramebuffers,
        };
        init::VulkanInit::InitWindowSwapchain(swapchainInitInfo);

        // Push the new swapchain to vectors
        _swapchains.push_back(newSwapchain);
        _swapchainImageFormats.push_back(newSwapchainImageFormat);
        _swapchainsImages.push_back(newSwapchainImages);
        _swapchainsImageViews.push_back(newSwapchainImageViews);
        _frameBuffers.push_back(newSwapchainFramebuffers);

        return match;
    }

    void SwapchainManager::DestroySwapchain(const core::Match &match)
    {
        super::DestroyItem(match);

        // Get index
        auto index = match.GetIndex();
        size_t lastIndex = _ids.size() - 1;

        // Destroy the framebuffers
        for (auto framebuffer : _frameBuffers[index])
        {
            _storage.vulkanDevice.destroyFramebuffer(framebuffer);
        }

        // Destroy the image views
        for (auto imageView : _swapchainsImageViews[index])
        {
            _storage.vulkanDevice.destroyImageView(imageView);
        }

        // Destroy the swapchain
        _storage.vulkanDevice.destroySwapchainKHR(_swapchains[index]);

        // Move the last item of the vectors
        if (index < lastIndex)
        {
            _swapchains[index] = _swapchains[lastIndex];
            _swapchainImageFormats[index] = _swapchainImageFormats[lastIndex];
            _swapchainsImages[index] = _swapchainsImages[lastIndex];
            _swapchainsImageViews[index] = _swapchainsImageViews[lastIndex];
            _frameBuffers[index] = _frameBuffers[lastIndex];
        }

        // Remove last elements
        _swapchains.pop_back();
        _swapchainImageFormats.pop_back();
        _swapchainsImages.pop_back();
        _swapchainsImageViews.pop_back();
        _frameBuffers.pop_back();
    }

    void SwapchainManager::RecreateWindowSwapchain(const core::Match &match, const vk::SurfaceKHR &surface, const core::WindowManager &windowManager, const vk::RenderPass &renderPass)
    {
        ADVANCED_CHECK(_initialized, NOT_INITIALIZED_ERROR);

        // Get index
        auto index = match.GetIndex();

        // Destroy the framebuffers
        for (auto framebuffer : _frameBuffers[index])
        {
            _storage.vulkanDevice.destroyFramebuffer(framebuffer);
        }

        // Destroy image views

        for (auto imageView : _swapchainsImageViews[index])
        {
            _storage.vulkanDevice.destroyImageView(imageView);
        }

        // Destroy old swapchain
        _storage.vulkanDevice.destroySwapchainKHR(_swapchains[index]);

        // Create new one at the same slot
        init::SwapchainInitInfo swapchainInitInfo{
            .device = _storage.vulkanDevice,
            .physicalDevice = _storage.vulkanPhysicalDevice,
            .surface = surface,
            .windowManager = windowManager,
            .renderPass = renderPass,
            .swapchain = &_swapchains[index],
            .swapchainImages = &_swapchainsImages[index],
            .swapchainImageFormat = &_swapchainImageFormats[index],
            .swapchainImageViews = &_swapchainsImageViews[index],
            .swapchainFramebuffers = &_frameBuffers[index],
        };
        init::VulkanInit::InitWindowSwapchain(swapchainInitInfo);
    }

    vk::SwapchainKHR SwapchainManager::GetSwapchain(const core::Match &match) const
    {
        ADVANCED_CHECK(_initialized, NOT_INITIALIZED_ERROR);

        return _swapchains[match.GetIndex()];
    }

    vk::Format SwapchainManager::GetSwapchainImageFormat(const core::Match &match) const
    {
        ADVANCED_CHECK(_initialized, NOT_INITIALIZED_ERROR);

        return _swapchainImageFormats[match.GetIndex()];
    }

    std::vector<vk::Image> SwapchainManager::GetSwapchainImages(const core::Match &match) const
    {
        ADVANCED_CHECK(_initialized, NOT_INITIALIZED_ERROR);

        return _swapchainsImages[match.GetIndex()];
    }

    std::vector<vk::ImageView> SwapchainManager::GetSwapchainImageViews(const core::Match &match) const
    {
        ADVANCED_CHECK(_initialized, NOT_INITIALIZED_ERROR);

        return _swapchainsImageViews[match.GetIndex()];
    }

    std::vector<vk::Framebuffer> SwapchainManager::GetFramebuffers(const core::Match &match) const {
        ADVANCED_CHECK(_initialized, NOT_INITIALIZED_ERROR);

        return _frameBuffers[match.GetIndex()];
    }
}