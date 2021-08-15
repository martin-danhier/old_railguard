#pragma once

#include "railguard/includes/Vma.h"
#include "railguard/includes/Vulkan.h"

namespace railguard::rendering::structs
{
    struct AllocatedBuffer
    {
        vk::Buffer buffer = nullptr;
        VmaAllocation allocation = nullptr;
        size_t size = 0;

        [[nodiscard]] bool IsNull() const {
            return allocation == nullptr;
        }
        [[nodiscard]] bool IsNotNull() const {
            return allocation != nullptr;
        }
    };

    struct AllocatedImage
    {
        vk::Image image = nullptr;
        VmaAllocation allocation{};
    };
} // namespace railguard::rendering::structs
