#pragma once

#include "../includes/Vma.h"
#include "../includes/Vulkan.h"
#include "../core/Match.h"
#include <unordered_map>
#include "Ids.h"

namespace railguard::rendering
{
    class AllocationManager
    {
    private:
        VmaAllocator _allocator = nullptr;

        // Buffers
        buffer_id_t _lastBufferId = 1;
        std::unordered_map<buffer_id_t, size_t> _bufferLookupMap;
        std::vector<buffer_id_t> _bufferIds;
        std::vector<vk::Buffer> _buffers;
        std::vector<VmaAllocation> _bufferAllocations;

        // Images
        image_id_t _lastImageId = 1;
        std::unordered_map<buffer_id_t, size_t> _imageLookupMap;
        std::vector<image_id_t> _imageIds;
        std::vector<vk::Image> _images;
        std::vector<VmaAllocation> _imageAllocations;

    public:
        AllocationManager(const vk::PhysicalDevice &physicalDevice, const vk::Device &device, const vk::Instance &instance, size_t bufferCapacity = 10, size_t imageCapacity = 10);
        void CleanUp();

        [[nodiscard]] core::Match LookupBuffer(buffer_id_t id);
        [[nodiscard]] core::CompleteMatch<buffer_id_t> CreateBuffer(size_t allocationSize, vk::BufferUsageFlags bufferUsage, VmaMemoryUsage memoryUsage);
        void DestroyBuffer(const core::Match &match);
        [[nodiscard]] const vk::Buffer &GetBuffer(const core::Match &match);
        [[nodiscard]] const VmaAllocation &GetBufferAllocation(const core::Match &match);
        [[nodiscard]] void *MapBuffer(const core::Match &match);
        void UnmapBuffer(const core::Match &match);

        template <class T>
        void CopyBufferToAllocation(const core::Match &match, const T *src, size_t size)
        {
            char *data = nullptr;

            // Get this allocation
            const auto index = match.GetIndex();
            const auto allocation = _bufferAllocations[index];

            // Map memory
            vmaMapMemory(_allocator, allocation, static_cast<void**>(&data));

            // Copy data to it
            memcpy(data, src, size);

            // Unmap memory
            vmaUnmapMemory(_allocator, allocation);
        }
    };
} // namespace railguard::rendering
