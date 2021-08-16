#include "railguard/rendering/AllocationManager.h"

#include "railguard/rendering/structs/AllocatedStructs.h"

namespace railguard::rendering
{
    AllocationManager::AllocationManager(const vk::PhysicalDevice &physicalDevice,
                                         const vk::Device &device,
                                         const vk::Instance &instance)
    {
        // Init allocator
        // Give VMA the functions pointers of vulkan functions
        // We need to do that since we load them dynamically
        VmaVulkanFunctions vulkanFunctions {
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
        VmaAllocatorCreateInfo allocatorInfo {
            .physicalDevice   = physicalDevice,
            .device           = device,
            .pVulkanFunctions = &vulkanFunctions,
            .instance         = instance,
        };
        vmaCreateAllocator(&allocatorInfo, &_allocator);
    }

    AllocationManager::~AllocationManager()
    {
        // Destroy allocator
        vmaDestroyAllocator(_allocator);
    }

    structs::AllocatedBuffer
        AllocationManager::CreateBuffer(size_t allocationSize, vk::BufferUsageFlags bufferUsage, VmaMemoryUsage memoryUsage)
    {
        // Create a new allocated buffer
        structs::AllocatedBuffer newBuffer {
            .buffer     = nullptr,
            .allocation = nullptr,
            .size       = allocationSize,
        };

        // Buffer info
        vk::BufferCreateInfo bufferCreateInfo {
            .size  = allocationSize,
            .usage = bufferUsage,
        };

        // Allocation info
        VmaAllocationCreateInfo allocationCreateInfo {};
        allocationCreateInfo.usage = memoryUsage;

        // Create the buffer
        vmaCreateBuffer(_allocator,
                        reinterpret_cast<VkBufferCreateInfo *>(&bufferCreateInfo),
                        &allocationCreateInfo,
                        reinterpret_cast<VkBuffer *>(&newBuffer.buffer),
                        &newBuffer.allocation,
                        nullptr);

        return newBuffer;
    }

    void AllocationManager::DestroyBuffer(structs::AllocatedBuffer &buffer)
    {
        // Destroy buffer
        vmaDestroyBuffer(_allocator, buffer.buffer, buffer.allocation);

        // Set values to 0 to make clear that this isn't valid anymore
        buffer.buffer     = nullptr;
        buffer.allocation = nullptr;
        buffer.size       = 0;
    }

    [[nodiscard]] void *AllocationManager::MapBuffer(const structs::AllocatedBuffer &buffer)
    {
        void *data = nullptr;
        vmaMapMemory(_allocator, buffer.allocation, &data);
        return data;
    }

    void AllocationManager::UnmapBuffer(const structs::AllocatedBuffer &buffer)
    {
        vmaUnmapMemory(_allocator, buffer.allocation);
    }

} // namespace railguard::rendering
