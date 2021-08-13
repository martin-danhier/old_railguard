#pragma once

#include "../includes/Vma.h"
#include "../includes/Vulkan.h"
#include "structs/AllocatedStructs.h"


namespace railguard::rendering
{
    class AllocationManager
    {
    private:
        VmaAllocator _allocator = nullptr;

#ifdef USE_ADVANCED_CHECKS
        bool _initialized = false;
#endif

    public:
        void Init(const vk::PhysicalDevice &physicalDevice, const vk::Device &device, const vk::Instance &instance);
        void CleanUp();

        [[nodiscard]] structs::AllocatedBuffer CreateBuffer(size_t allocationSize, vk::BufferUsageFlags bufferUsage, VmaMemoryUsage memoryUsage);
        void DestroyBuffer(structs::AllocatedBuffer &buffer);
        [[nodiscard]] void *MapBuffer(const structs::AllocatedBuffer &buffer);
        void UnmapBuffer(const structs::AllocatedBuffer &buffer);

        template <class T>
        void CopyBufferToAllocation(const structs::AllocatedBuffer &buffer, const T *src, size_t size)
        {

            // Map memory
            char *data = static_cast<char *>(MapBuffer(buffer));

            // Copy data to it
            memcpy(data, src, size);

            // Unmap memory
            UnmapBuffer(buffer);
        }
    };
} // namespace railguard::rendering
