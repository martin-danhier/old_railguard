#pragma once

#include "../../includes/Vma.h"
#include "../../includes/Vulkan.h"

namespace railguard::rendering::structs
{
    struct AllocatedBuffer
    {
        vk::Buffer buffer;
        VmaAllocation allocation{};
        size_t size;
    };

    struct AllocatedImage
    {
        vk::Image image;
        VmaAllocation allocation{};
    };
} // namespace railguard::rendering::structs
