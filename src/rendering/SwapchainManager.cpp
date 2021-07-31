#include "../../include/rendering/SwapchainManager.h"
#include "../../include/rendering/init/VulkanInit.h"
#include "../../include/utils/AdvancedCheck.h"
#include "../../include/rendering/Settings.h"

#ifdef USE_ADVANCED_CHECKS
// Define local error messages
#define NOT_INITIALIZED_ERROR "SwapchainManager should be initialized with Init before calling this method."
#endif

namespace railguard::rendering
{

    void SwapchainManager::Init(structs::FullDeviceStorage storage, size_t defaultCapacity)
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

    uint32_t SwapchainManager::RequestNextImageIndex(const core::Match &match, const vk::Semaphore &presentSemaphore)
    {
        // Get index of swapchain
        auto index = match.GetIndex();

        // Call the swapchain and request the next image.
        auto nextImageResult = _storage.vulkanDevice.acquireNextImageKHR(_swapchains[index], SEMAPHORE_TIMEOUT, presentSemaphore, nullptr);

        switch (nextImageResult.result)
        {
        case vk::Result::eSuccess:
            // Success, keep it
            return nextImageResult.value;

        case vk::Result::eTimeout:
        case vk::Result::eNotReady:
        case vk::Result::eSuboptimalKHR:
            // These cases should be handled.
            // For now, we will fall back to the error below.
            // TODO one day, perhaps

        default:
            // Default: error. vk-hpp already throws an exception, so there is nothing
            // to do
            throw std::runtime_error("Error while getting next image");
        }
    }

    void SwapchainManager::PresentImage(const core::Match &match, uint32_t imageIndex, const vk::Semaphore &renderSemaphore, const vk::Queue &graphicsQueue)
    {
        // Get index
        auto index = match.GetIndex();

        // Present the image on the screen
        vk::PresentInfoKHR presentInfo{
            // Wait until the rendering is complete
            .waitSemaphoreCount = 1,
            .pWaitSemaphores = &renderSemaphore,
            // Specify the swapchain to present
            .swapchainCount = 1,
            .pSwapchains = &_swapchains[index],
            // Specify the index of the image
            .pImageIndices = &imageIndex,
        };
        auto result = graphicsQueue.presentKHR(presentInfo);
        switch (result)
        {
        case vk::Result::eSuccess:
        case vk::Result::eSuboptimalKHR:
            // Success
            break;

        default:
            // Error
            throw std::runtime_error("Failed to present image.");
        }
    }

    vk::SwapchainKHR SwapchainManager::GetSwapchain(const core::Match &match) const
    {
        return _swapchains[match.GetIndex()];
    }

    vk::Format SwapchainManager::GetSwapchainImageFormat(const core::Match &match) const
    {
        return _swapchainImageFormats[match.GetIndex()];
    }

    std::vector<vk::Image> SwapchainManager::GetSwapchainImages(const core::Match &match) const
    {
        return _swapchainsImages[match.GetIndex()];
    }

    std::vector<vk::ImageView> SwapchainManager::GetSwapchainImageViews(const core::Match &match) const
    {
        return _swapchainsImageViews[match.GetIndex()];
    }

    std::vector<vk::Framebuffer> SwapchainManager::GetFramebuffers(const core::Match &match) const
    {
        return _frameBuffers[match.GetIndex()];
    }
}