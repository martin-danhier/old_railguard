#pragma once
#include <cstdint>

// Forward declarations
using std::uint32_t;
#define VK_DEFINE_HANDLE(object) typedef struct object##_T *(object);
VK_DEFINE_HANDLE(VmaAllocator)
VK_DEFINE_HANDLE(VkPhysicalDevice)
VK_DEFINE_HANDLE(VkDevice)
VK_DEFINE_HANDLE(VkInstance)
typedef uint32_t VkFlags;
typedef VkFlags VkBufferUsageFlags;
typedef unsigned long VmaMemoryUsage;

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
        AllocationManager(const VkPhysicalDevice &physicalDevice, const VkDevice &device, const VkInstance &instance);
        ~AllocationManager();

        [[nodiscard]] structs::AllocatedBuffer
            CreateBuffer(size_t allocationSize, VkBufferUsageFlags bufferUsage, VmaMemoryUsage memoryUsage);
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
