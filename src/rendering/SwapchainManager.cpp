#include <railguard/core/WindowManager.h>
#include <railguard/rendering/FrameManager.h>
#include <railguard/rendering/SwapchainManager.h>
#include <railguard/rendering/init/SwapchainInitInfo.h>
#include <railguard/rendering/init/VulkanInit.h>
#include <railguard/utils/AdvancedCheck.h>

#ifdef USE_ADVANCED_CHECKS
    // Define local error messages
    #define NOT_INITIALIZED_ERROR "SwapchainManager should be initialized with Init before calling this method."
#endif

namespace railguard::rendering
{
    SwapchainManager::SwapchainManager(SwapchainManagerStorage storage, size_t defaultCapacity) : super(storage, defaultCapacity)
    {
        // Init vectors that weren't initialized by parent
        _swapchains.reserve(defaultCapacity);
        _swapchainImageFormats.reserve(defaultCapacity);
        _swapchainsImages.reserve(defaultCapacity);
        _swapchainsImageViews.reserve(defaultCapacity);
        _frameBuffers.reserve(defaultCapacity);
        _viewportExtents.reserve(defaultCapacity);
        _lastTimeSubmitted.reserve(defaultCapacity);
        _imageIndex.reserve(defaultCapacity);
    }

    void SwapchainManager::CleanUp()
    {
        // Clean up without clearing vectors
        // Destroy the framebuffers
        for (const auto &framebufferVector : _frameBuffers)
        {
            for (auto framebuffer : framebufferVector)
            {
                _storage.vulkanDevice.destroyFramebuffer(framebuffer);
            }
        }

        // Destroy image views
        for (const auto &imageViewVector : _swapchainsImageViews)
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
    }

    void SwapchainManager::Clear()
    {
        super::Clear();

        // Clean up everything
        CleanUp();

        // Clear everything
        _swapchains.clear();
        _swapchainImageFormats.clear();
        _swapchainsImageViews.clear();
        _swapchainsImages.clear();
        _viewportExtents.clear();
        _lastTimeSubmitted.clear();
        _imageIndex.clear();
    }

    core::CompleteMatch<swapchain_id_t> SwapchainManager::CreateWindowSwapchain(const vk::SurfaceKHR &surface, const core::WindowManager &windowManager, const vk::RenderPass &renderPass)
    {
        auto match = super::CreateItem();

        vk::Extent2D windowExtent = windowManager.GetWindowExtent();

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
            .windowExtent = windowExtent,
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
        _viewportExtents.push_back(windowExtent);
        _lastTimeSubmitted.push_back(0);
        _imageIndex.push_back(0);

        return match;
    }

    void SwapchainManager::DestroySwapchain(const core::Match &match)
    {
        // Get index
        auto index = match.GetIndex();
        const size_t lastIndex = _ids.size() - 1;

        // Run boilerplate deletion
        super::DestroyItem(match);

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
            _viewportExtents[index] = _viewportExtents[lastIndex];
            _lastTimeSubmitted[index] = _lastTimeSubmitted[lastIndex];
            _imageIndex[index] = _imageIndex[lastIndex];
        }

        // Remove last elements
        _swapchains.pop_back();
        _swapchainImageFormats.pop_back();
        _swapchainsImages.pop_back();
        _swapchainsImageViews.pop_back();
        _frameBuffers.pop_back();
        _viewportExtents.pop_back();
        _lastTimeSubmitted.pop_back();
        _imageIndex.pop_back();
    }

    void SwapchainManager::RecreateWindowSwapchain(const core::Match &match, const vk::SurfaceKHR &surface, const vk::Extent2D &newExtent, const vk::RenderPass &renderPass)
    {
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
            .windowExtent = newExtent,
            .renderPass = renderPass,
            .swapchain = &_swapchains[index],
            .swapchainImages = &_swapchainsImages[index],
            .swapchainImageFormat = &_swapchainImageFormats[index],
            .swapchainImageViews = &_swapchainsImageViews[index],
            .swapchainFramebuffers = &_frameBuffers[index],
        };
        init::VulkanInit::InitWindowSwapchain(swapchainInitInfo);

        // Update extent
        _viewportExtents[index] = newExtent;
    }

    uint32_t SwapchainManager::RequestNextImageIndex(const core::Match &match)
    {
        // Get index of swapchain
        auto index = match.GetIndex();

        auto currentFrameNumber = _storage.frameManager->GetFrameNumber();

        // First time queried this frame
        if (currentFrameNumber != _lastTimeSubmitted[index])
        {
            auto presentSemaphore = *_storage.frameManager->GetCurrentPresentSemaphore();

            // Call the swapchain and request the next image.
            auto nextImageResult = _storage.vulkanDevice.acquireNextImageKHR(_swapchains[index], SEMAPHORE_TIMEOUT, presentSemaphore, nullptr);

            if (nextImageResult.result == vk::Result::eSuccess)
            {
                // Success, keep it
                auto imageIndex = nextImageResult.value;

                // Cache it
                _imageIndex[index] = imageIndex;
                _lastTimeSubmitted[index] = currentFrameNumber;

                return imageIndex;

                // TODO handle these cases:
                // vk::Result::eTimeout
                // vk::Result::eNotReady
                // vk::Result::eSuboptimalKHR
            }
            else
            {

                // Default: error. vk-hpp already throws an exception, so there is nothing
                // to do
                throw std::runtime_error("Error while getting next image");
            }
        }
        // It was already queried this frame. Get it from cache
        else
        {
            return _imageIndex[index];
        }
    }

    void SwapchainManager::PresentUsedImages(const vk::Queue &graphicsQueue) {
        // Get frame number
        auto currentFrameNumber = _storage.frameManager->GetFrameNumber();

        // For each swapchain that was used this frame
        for (uint32_t i = 0; i < _lastTimeSubmitted.size(); i++) {
            if (currentFrameNumber == _lastTimeSubmitted[i]) {
                // Present it
                PresentImage(i, graphicsQueue);
            }
        }
    }

    void SwapchainManager::PresentImage(const core::Match &match, const vk::Queue &graphicsQueue)
    {
        // Get index
        auto index = match.GetIndex();
        PresentImage(index, graphicsQueue);
    }

    void SwapchainManager::PresentImage(size_t index, const vk::Queue &graphicsQueue)
    {

#ifdef USE_ADVANCED_CHECKS
        // Get frame number
        auto currentFrameNumber = _storage.frameManager->GetFrameNumber();
        ADVANCED_CHECK(currentFrameNumber == _lastTimeSubmitted[index], "This swapchain shouldn't be presented, no image was requested this frame.");
#endif

        // Present the image on the screen
        vk::PresentInfoKHR presentInfo{
            // Wait until the rendering is complete
            .waitSemaphoreCount = 1,
            .pWaitSemaphores = _storage.frameManager->GetCurrentRenderSemaphore(),
            // Specify the swapchain to present
            .swapchainCount = 1,
            .pSwapchains = &_swapchains[index],
            // Specify the index of the image
            .pImageIndices = &_imageIndex[index],
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

    vk::Extent2D SwapchainManager::GetViewportExtent(const core::Match &match) const
    {
        return _viewportExtents[match.GetIndex()];
    }

    SwapchainManager::~SwapchainManager()
    {
        CleanUp();
    }
} // namespace railguard::rendering