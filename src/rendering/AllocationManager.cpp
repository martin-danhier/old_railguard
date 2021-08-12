#include "../../include/rendering/AllocationManager.h"

namespace railguard::rendering
{
    AllocationManager::AllocationManager(const vk::PhysicalDevice &physicalDevice, const vk::Device &device, const vk::Instance &instance, size_t bufferCapacity, size_t imageCapacity)
    {
        // Init allocator
        // Give VMA the functions pointers of vulkan functions
        // We need to do that since we load them dynamically
        VmaVulkanFunctions vulkanFunctions{
            VULKAN_HPP_DEFAULT_DISPATCHER.vkGetPhysicalDeviceProperties,
            VULKAN_HPP_DEFAULT_DISPATCHER.vkGetPhysicalDeviceMemoryProperties,
            VULKAN_HPP_DEFAULT_DISPATCHER.vkAllocateMemory,
            VULKAN_HPP_DEFAULT_DISPATCHER.vkFreeMemory,
            VULKAN_HPP_DEFAULT_DISPATCHER.vkMapMemory,
            VULKAN_HPP_DEFAULT_DISPATCHER.vkUnmapMemory,
            VULKAN_HPP_DEFAULT_DISPATCHER.vkFlushMappedMemoryRanges,
            VULKAN_HPP_DEFAULT_DISPATCHER.vkInvalidateMappedMemoryRanges,
            VULKAN_HPP_DEFAULT_DISPATCHER.vkBindBufferMemory,
            VULKAN_HPP_DEFAULT_DISPATCHER.vkBindImageMemory,
            VULKAN_HPP_DEFAULT_DISPATCHER.vkGetBufferMemoryRequirements,
            VULKAN_HPP_DEFAULT_DISPATCHER.vkGetImageMemoryRequirements,
            VULKAN_HPP_DEFAULT_DISPATCHER.vkCreateBuffer,
            VULKAN_HPP_DEFAULT_DISPATCHER.vkDestroyBuffer,
            VULKAN_HPP_DEFAULT_DISPATCHER.vkCreateImage,
            VULKAN_HPP_DEFAULT_DISPATCHER.vkDestroyImage,
            VULKAN_HPP_DEFAULT_DISPATCHER.vkCmdCopyBuffer,
            VULKAN_HPP_DEFAULT_DISPATCHER.vkGetBufferMemoryRequirements2KHR,
            VULKAN_HPP_DEFAULT_DISPATCHER.vkGetImageMemoryRequirements2KHR,
            VULKAN_HPP_DEFAULT_DISPATCHER.vkBindBufferMemory2KHR,
            VULKAN_HPP_DEFAULT_DISPATCHER.vkBindImageMemory2KHR,
            VULKAN_HPP_DEFAULT_DISPATCHER.vkGetPhysicalDeviceMemoryProperties2KHR,
        };
        VmaAllocatorCreateInfo allocatorInfo{
            .physicalDevice = physicalDevice,
            .device = device,
            .pVulkanFunctions = &vulkanFunctions,
            .instance = instance,
        };
        vmaCreateAllocator(&allocatorInfo, &_allocator);

        // Reserve space in vectors
        _bufferIds.reserve(bufferCapacity);
        _buffers.reserve(bufferCapacity);
        _bufferLookupMap.reserve(bufferCapacity);
        _bufferAllocations.reserve(bufferCapacity);

        _imageLookupMap.reserve(imageCapacity);
        _imageIds.reserve(imageCapacity);
        _images.reserve(imageCapacity);
        _imageAllocations.reserve(imageCapacity);
    }

    void AllocationManager::CleanUp()
    {
        // Destroy allocator
        vmaDestroyAllocator(_allocator);
    }

    core::Match AllocationManager::LookupBuffer(buffer_id_t id)
    {
        size_t index = _bufferLookupMap.at(id);
        return core::Match(index);
    }

    [[nodiscard]] core::CompleteMatch<buffer_id_t> AllocationManager::CreateBuffer(size_t allocationSize, vk::BufferUsageFlags bufferUsage, VmaMemoryUsage memoryUsage)
    {

        // Add new buffer
        auto index = _bufferIds.size();
        auto newId = _lastBufferId++;
        _bufferLookupMap[newId] = index;
        _bufferIds.push_back(newId);
        _buffers.push_back(nullptr);
        _bufferAllocations.push_back(nullptr);

        // Buffer info
        vk::BufferCreateInfo bufferCreateInfo{
            .size = allocationSize,
            .usage = bufferUsage,
        };

        // Allocation info
        VmaAllocationCreateInfo allocationCreateInfo{};
        allocationCreateInfo.usage = memoryUsage;

        // Create the buffer
        vmaCreateBuffer(_allocator, reinterpret_cast<VkBufferCreateInfo *>(&bufferCreateInfo),
                        &allocationCreateInfo, reinterpret_cast<VkBuffer *>(&_buffers[index]),
                        &_bufferAllocations[index], nullptr);

        return core::CompleteMatch<buffer_id_t>(index + 1, newId);
    }

    void AllocationManager::DestroyBuffer(const core::Match &match)
    {
        auto index = match.GetIndex();
        auto lastIndex = _bufferIds.size() - 1;
        auto destroyedId = _bufferIds[index];

        // Destroy buffer
        vmaDestroyBuffer(_allocator, _buffers[index], _bufferAllocations[index]);

        // Move last element if needed
        if (index < lastIndex)
        {
            auto movedId = _bufferIds[lastIndex];
            _bufferLookupMap[movedId] = index;
            _bufferIds[index] = _bufferIds[lastIndex];
            _buffers[index] = _buffers[lastIndex];
            _bufferAllocations[index] = _bufferAllocations[lastIndex];
        }

        _bufferLookupMap.erase(destroyedId);
        _buffers.pop_back();
        _bufferAllocations.pop_back();
        _bufferIds.pop_back();
    }

    [[nodiscard]] const vk::Buffer &AllocationManager::GetBuffer(const core::Match &match)
    {
        return _buffers[match.GetIndex()];
    }

    [[nodiscard]] const VmaAllocation &AllocationManager::GetBufferAllocation(const core::Match &match)
    {
        return _bufferAllocations[match.GetIndex()];
    }

    [[nodiscard]] void *AllocationManager::MapBuffer(const core::Match &match)
    {
        void *buffer = nullptr;
        const auto index = match.GetIndex();
        vmaMapMemory(_allocator, _bufferAllocations[index], &buffer);
        return buffer;
    }

    void AllocationManager::UnmapBuffer(const core::Match &match)
    {
        const auto index = match.GetIndex();
        vmaUnmapMemory(_allocator, _bufferAllocations[index]);
    }

} // namespace railguard::rendering
