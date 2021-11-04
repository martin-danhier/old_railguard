#pragma once

#define VK_DEFINE_HANDLE(object) typedef struct object##_T *object;
VK_DEFINE_HANDLE(VkBuffer)
VK_DEFINE_HANDLE(VmaAllocation)
VK_DEFINE_HANDLE(VkImage)

namespace railguard::rendering::structs
{
    struct AllocatedBuffer
    {
        VkBuffer buffer          = nullptr;
        VmaAllocation allocation = nullptr;
        size_t size              = 0;

        [[nodiscard]] bool IsNull() const
        {
            return allocation == nullptr;
        }
        [[nodiscard]] bool IsNotNull() const
        {
            return allocation != nullptr;
        }
    };

    struct AllocatedImage
    {
        VkImage image = nullptr;
        VmaAllocation allocation {};
    };
} // namespace railguard::rendering::structs
