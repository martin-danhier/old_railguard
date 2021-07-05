#include "../../../include/rendering/SwapchainManager.h"
#include "../../../include/rendering/init/VulkanInit.h"

namespace railguard::rendering
{

    SwapchainManager::SwapchainManager(size_t defaultCapacity)
    {
        // Allocate space in the vectors
        _id.reserve(defaultCapacity);
        _swapchain.reserve(defaultCapacity);
        _swapchainImageFormat.reserve(defaultCapacity);
        _swapchainImages.reserve(defaultCapacity);
        _swapchainImageViews.reserve(defaultCapacity);
    }

    swapchain_id_t SwapchainManager::CreateWindowSwapchain(const vk::Device &device, const vk::PhysicalDevice &physicalDevice, const vk::SurfaceKHR &surface, const core::WindowManager &windowManager)
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
            .device = device,
            .physicalDevice = physicalDevice,
            .surface = surface,
            .windowManager = windowManager,
            .swapchain = &newSwapchain,
            .swapchainImages = &newSwapchainImages,
            .swapchainImageFormat = &newSwapchainImageFormat,
            .swapchainImageViews = &newSwapchainImageViews};
        init::VulkanInit::InitWindowSwapchain(swapchainInitInfo);

        // Push the new swapchain to vectors
        _id.push_back(newId);
        _swapchain.push_back(newSwapchain);
        _swapchainImageFormat.push_back(newSwapchainImageFormat);
        _swapchainImages.push_back(newSwapchainImages);
        _swapchainImageViews.push_back(newSwapchainImageViews);

        // Save id in the map
        _idLookupMap[newId] = _id.size();

        return newId;
    }

    core::Match SwapchainManager::LookupId(swapchain_id_t id)
    {
        size_t index = _idLookupMap[id];
        return core::Match(index);
    }

    void SwapchainManager::DestroySwapchain(const vk::Device &device, const core::Match &match)
    {
        // Get index
        swapchain_id_t index = match.GetIndex();

        // Destroy the swapchain
        device.destroySwapchainKHR(_swapchain[index]);

        // Remove entry from map
        _idLookupMap.erase(_id[index]);

        // Move the last item of the vectors
        size_t lastIndex = _id.size() - 1;
        if (index < lastIndex)
        {
            swapchain_id_t movedId = _id[lastIndex];
            _id[index] = movedId;
            _swapchain[index] = _swapchain[lastIndex];
            _swapchainImageFormat[index] = _swapchainImageFormat[lastIndex];
            _swapchainImages[index] = _swapchainImages[lastIndex];
            _swapchainImageViews[index] = _swapchainImageViews[lastIndex];
            // Update id in map
            _idLookupMap[movedId] = index;
        }

        // Remove last elements
        _id.pop_back();
        _swapchain.pop_back();
        _swapchainImageFormat.pop_back();
        _swapchainImages.pop_back();
        _swapchainImageViews.pop_back();
    }

    void SwapchainManager::RecreateWindowSwapchain(const core::Match &match, const vk::Device &device, const vk::PhysicalDevice &physicalDevice, const vk::SurfaceKHR &surface, const core::WindowManager &windowManager) {

        // Get index
        auto index = match.GetIndex();

        // Destroy old swapchain
        device.destroySwapchainKHR(_swapchain[index]);

        // Create new one at the same slot
        init::SwapchainInitInfo swapchainInitInfo{
            .device = device,
            .physicalDevice = physicalDevice,
            .surface = surface,
            .windowManager = windowManager,
            .swapchain = &_swapchain[index],
            .swapchainImages = &_swapchainImages[index],
            .swapchainImageFormat = &_swapchainImageFormat[index],
            .swapchainImageViews = &_swapchainImageViews[index]
        };
        init::VulkanInit::InitWindowSwapchain(swapchainInitInfo);

    }

    vk::SwapchainKHR SwapchainManager::GetSwapchain(const core::Match &match) const {
        return _swapchain[match.GetIndex()];
    }

    vk::Format SwapchainManager::GetSwapchainImageFormat(const core::Match &match) const {
        return _swapchainImageFormat[match.GetIndex()];
    }

    std::vector<vk::Image> SwapchainManager::GetSwapchainImages(const core::Match &match) const {
        return _swapchainImages[match.GetIndex()];
    }

    std::vector<vk::ImageView> SwapchainManager::GetSwapchainImageViews(const core::Match &match) const {
        return _swapchainImageViews[match.GetIndex()];
    }
}