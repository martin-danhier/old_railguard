#pragma once

#include "railguard/includes/Vma.h"

// Forward declarations
// Avoids a vulkan import in the header
namespace vk
{
    class PhysicalDevice;
    class Device;
    class Instance;
    enum class BufferUsageFlagBits : uint32_t;
    template<typename T>
    class Flags;
    typedef Flags<BufferUsageFlagBits> BufferUsageFlags;
} // namespace vk

namespace railguard::rendering
{
    namespace structs
    {
        struct AllocatedBuffer;
    }

    class AllocationManager
    {
      private:
        VmaAllocator _allocator = nullptr;

      public:
        AllocationManager(const vk::PhysicalDevice &physicalDevice, const vk::Device &device, const vk::Instance &instance);
        ~AllocationManager();

        [[nodiscard]] structs::AllocatedBuffer
            CreateBuffer(size_t allocationSize, vk::BufferUsageFlags bufferUsage, VmaMemoryUsage memoryUsage);
        void DestroyBuffer(structs::AllocatedBuffer &buffer);
        [[nodiscard]] void *MapBuffer(const structs::AllocatedBuffer &buffer);
        void UnmapBuffer(const structs::AllocatedBuffer &buffer);

        template<class T>
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
