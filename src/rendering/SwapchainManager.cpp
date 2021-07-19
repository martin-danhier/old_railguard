#include "../../../include/rendering/SwapchainManager.h"
#include "../../../include/rendering/init/VulkanInit.h"
#include "../../include/utils/DebugAssert.h"

#ifndef NDEBUG
// Define local error messages
#define INITIALIZED_TWICE_ERROR "SwapchainManager should not be initialized twice."
#define NOT_INITIALIZED_ERROR "SwapchainManager should be initialized with Init before calling this method."
#endif

namespace railguard::rendering
{

    void SwapchainManager::Init(const vk::Device &device, const vk::PhysicalDevice &physicalDevice, size_t defaultCapacity)
    {
        DEBUG_ASSERT(!_initialized, INITIALIZED_TWICE_ERROR);

        _vulkanDevice = vk::Device(device);
        _vulkanPhysicalDevice = vk::PhysicalDevice(physicalDevice);

        // Allocate space in the vectors
        _ids.reserve(defaultCapacity);
        _swapchains.reserve(defaultCapacity);
        _swapchainImageFormats.reserve(defaultCapacity);
        _swapchainsImages.reserve(defaultCapacity);
        _swapchainsImageViews.reserve(defaultCapacity);
        _frameBuffers.reserve(defaultCapacity);

#ifndef NDEBUG
        _initialized = true;
#endif
    }

    void SwapchainManager::Clear()
    {
        DEBUG_ASSERT(_initialized, NOT_INITIALIZED_ERROR);

        // Destroy the framebuffers
        for (auto framebufferVector : _frameBuffers)
        {
            for (auto framebuffer : framebufferVector)
            {
                _vulkanDevice.destroyFramebuffer(framebuffer);
            }
        }

        // Destroy image views
        for (auto imageViewVector : _swapchainsImageViews)
        {
            for (auto imageView : imageViewVector)
            {
                _vulkanDevice.destroyImageView(imageView);
            }
        }

        // We need to destroy every swapchain
        // No need to do other operations on the vectors, since they will be destroyed anyway
        for (auto swapchain : _swapchains)
        {
            _vulkanDevice.destroySwapchainKHR(swapchain);
        }

        // Clear everything
        _ids.clear();
        _swapchains.clear();
        _swapchainImageFormats.clear();
        _swapchainsImageViews.clear();
        _swapchainsImages.clear();
        _idLookupMap.clear();
    }

    swapchain_id_t SwapchainManager::CreateWindowSwapchain(const vk::SurfaceKHR &surface, const core::WindowManager &windowManager, const vk::RenderPass &renderPass)
    {
        DEBUG_ASSERT(_initialized, NOT_INITIALIZED_ERROR);

        // Get new id for the swapchain
        swapchain_id_t newId = _lastUsedId++;

        // Create temporary destination variables
        vk::SwapchainKHR newSwapchain;
        vk::Format newSwapchainImageFormat;
        std::vector<vk::Image> newSwapchainImages;
        std::vector<vk::ImageView> newSwapchainImageViews;
        std::vector<vk::Framebuffer> newSwapchainFramebuffers;

        // Create the swapchain
        init::SwapchainInitInfo swapchainInitInfo{
            .device = _vulkanDevice,
            .physicalDevice = _vulkanPhysicalDevice,
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
        _ids.push_back(newId);
        _swapchains.push_back(newSwapchain);
        _swapchainImageFormats.push_back(newSwapchainImageFormat);
        _swapchainsImages.push_back(newSwapchainImages);
        _swapchainsImageViews.push_back(newSwapchainImageViews);
        _frameBuffers.push_back(newSwapchainFramebuffers);

        // Save id in the map
        _idLookupMap[newId] = _ids.size();

        return newId;
    }

    core::Match SwapchainManager::LookupId(swapchain_id_t id)
    {
        DEBUG_ASSERT(_initialized, NOT_INITIALIZED_ERROR);

        size_t index = _idLookupMap[id];
        return core::Match(index);
    }

    void SwapchainManager::DestroySwapchain(const core::Match &match)
    {
        DEBUG_ASSERT(_initialized, NOT_INITIALIZED_ERROR);

        // Get index
        swapchain_id_t index = static_cast<swapchain_id_t>(match.GetIndex());

        // Destroy the framebuffers
        for (auto framebuffer : _frameBuffers[index])
        {
            _vulkanDevice.destroyFramebuffer(framebuffer);
        }

        // Destroy the image views
        for (auto imageView : _swapchainsImageViews[index])
        {
            _vulkanDevice.destroyImageView(imageView);
        }

        // Destroy the swapchain
        _vulkanDevice.destroySwapchainKHR(_swapchains[index]);

        // Remove entry from map
        _idLookupMap.erase(_ids[index]);

        // Move the last item of the vectors
        size_t lastIndex = _ids.size() - 1;
        if (index < lastIndex)
        {
            swapchain_id_t movedId = _ids[lastIndex];
            _ids[index] = movedId;
            _swapchains[index] = _swapchains[lastIndex];
            _swapchainImageFormats[index] = _swapchainImageFormats[lastIndex];
            _swapchainsImages[index] = _swapchainsImages[lastIndex];
            _swapchainsImageViews[index] = _swapchainsImageViews[lastIndex];
            _frameBuffers[index] = _frameBuffers[lastIndex];
            // Update id in map
            _idLookupMap[movedId] = index;
        }

        // Remove last elements
        _ids.pop_back();
        _swapchains.pop_back();
        _swapchainImageFormats.pop_back();
        _swapchainsImages.pop_back();
        _swapchainsImageViews.pop_back();
        _frameBuffers.pop_back();
    }

    void SwapchainManager::RecreateWindowSwapchain(const core::Match &match, const vk::SurfaceKHR &surface, const core::WindowManager &windowManager, const vk::RenderPass &renderPass)
    {
        DEBUG_ASSERT(_initialized, NOT_INITIALIZED_ERROR);

        // Get index
        auto index = match.GetIndex();

        // Destroy the framebuffers
        for (auto framebuffer : _frameBuffers[index])
        {
            _vulkanDevice.destroyFramebuffer(framebuffer);
        }

        // Destroy image views

        for (auto imageView : _swapchainsImageViews[index])
        {
            _vulkanDevice.destroyImageView(imageView);
        }

        // Destroy old swapchain
        _vulkanDevice.destroySwapchainKHR(_swapchains[index]);

        // Create new one at the same slot
        init::SwapchainInitInfo swapchainInitInfo{
            .device = _vulkanDevice,
            .physicalDevice = _vulkanPhysicalDevice,
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
        DEBUG_ASSERT(_initialized, NOT_INITIALIZED_ERROR);

        return _swapchains[match.GetIndex()];
    }

    vk::Format SwapchainManager::GetSwapchainImageFormat(const core::Match &match) const
    {
        DEBUG_ASSERT(_initialized, NOT_INITIALIZED_ERROR);

        return _swapchainImageFormats[match.GetIndex()];
    }

    std::vector<vk::Image> SwapchainManager::GetSwapchainImages(const core::Match &match) const
    {
        DEBUG_ASSERT(_initialized, NOT_INITIALIZED_ERROR);

        return _swapchainsImages[match.GetIndex()];
    }

    std::vector<vk::ImageView> SwapchainManager::GetSwapchainImageViews(const core::Match &match) const
    {
        DEBUG_ASSERT(_initialized, NOT_INITIALIZED_ERROR);

        return _swapchainsImageViews[match.GetIndex()];
    }

    std::vector<vk::Framebuffer> SwapchainManager::GetFramebuffers(const core::Match &match) const {
        DEBUG_ASSERT(_initialized, NOT_INITIALIZED_ERROR);

        return _frameBuffers[match.GetIndex()];
    }
}