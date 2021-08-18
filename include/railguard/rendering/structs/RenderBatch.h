#pragma once

#include "railguard/includes/Vulkan.h"

namespace railguard::rendering::structs
{
    struct RenderBatch
    {
        size_t offset;
        size_t count;
        vk::Pipeline pipeline;
    };
} // namespace railguard::rendering::structs
