#include "../../../include/rendering/SwapchainManager.h"
#include "../../../include/rendering/init/VulkanInit.h"

namespace railguard::rendering
{

    SwapchainManager::SwapchainManager() : _vulkanDevice(nullptr),
                                           _vulkanPhysicalDevice(nullptr) {}

    SwapchainManager::SwapchainManager(vk::Device device, vk::PhysicalDevice physicalDevice, size_t defaultCapacity)
    {
        _vulkanDevice = device;
        _vulkanPhysicalDevice = physicalDevice;

        // Allocate space in the vectors
        _ids.reserve(defaultCapacity);
        _swapchains.reserve(defaultCapacity);
        _swapchainImageFormats.reserve(defaultCapacity);
        _swapchainsImages.reserve(defaultCapacity);
        _swapchainsImageViews.reserve(defaultCapacity);
    }

    void SwapchainManager::Clear()
    {

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
        // We also iterate in reverse to destroy the first created swapchains last
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

    swapchain_id_t SwapchainManager::CreateWindowSwapchain(const vk::SurfaceKHR &surface, const core::WindowManager &windowManager)
    {
        // Get new id for the swapchain
        swapchain_id_t newId = _lastUsedId++;

        // Create temporary destination variables
        vk::SwapchainKHR newSwapchain;
        vk::Format newSwapchainImageFormat;
        std::vector<vk::Image> newSwapchainImages;
        std::vector<vk::ImageView> newSwapchainImageViews;

        // Create the swapchain
        init::SwapchainInitInfo swapchainInitInfo{
            .device = _vulkanDevice,
            .physicalDevice = _vulkanPhysicalDevice,
            .surface = surface,
            .windowManager = windowManager,
            .swapchain = &newSwapchain,
            .swapchainImages = &newSwapchainImages,
            .swapchainImageFormat = &newSwapchainImageFormat,
            .swapchainImageViews = &newSwapchainImageViews};
        init::VulkanInit::InitWindowSwapchain(swapchainInitInfo);

        // Push the new swapchain to vectors
        _ids.push_back(newId);
        _swapchains.push_back(newSwapchain);
        _swapchainImageFormats.push_back(newSwapchainImageFormat);
        _swapchainsImages.push_back(newSwapchainImages);
        _swapchainsImageViews.push_back(newSwapchainImageViews);

        // Save id in the map
        _idLookupMap[newId] = _ids.size();

        return newId;
    }

    core::Match SwapchainManager::LookupId(swapchain_id_t id)
    {
        size_t index = _idLookupMap[id];
        return core::Match(index);
    }

    void SwapchainManager::DestroySwapchain(const core::Match &match)
    {
        // Get index
        swapchain_id_t index = static_cast<swapchain_id_t>(match.GetIndex());

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
            // Update id in map
            _idLookupMap[movedId] = index;
        }

        // Remove last elements
        _ids.pop_back();
        _swapchains.pop_back();
        _swapchainImageFormats.pop_back();
        _swapchainsImages.pop_back();
        _swapchainsImageViews.pop_back();
    }

    void SwapchainManager::RecreateWindowSwapchain(const core::Match &match, const vk::SurfaceKHR &surface, const core::WindowManager &windowManager)
    {

        // Get index
        auto index = match.GetIndex();

        // Destroy old swapchain
        _vulkanDevice.destroySwapchainKHR(_swapchains[index]);

        // Create new one at the same slot
        init::SwapchainInitInfo swapchainInitInfo{
            .device = _vulkanDevice,
            .physicalDevice = _vulkanPhysicalDevice,
            .surface = surface,
            .windowManager = windowManager,
            .swapchain = &_swapchains[index],
            .swapchainImages = &_swapchainsImages[index],
            .swapchainImageFormat = &_swapchainImageFormats[index],
            .swapchainImageViews = &_swapchainsImageViews[index]};
        init::VulkanInit::InitWindowSwapchain(swapchainInitInfo);
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
}