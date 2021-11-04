#pragma once

#define VK_DEFINE_HANDLE(object) typedef struct object##_T* object;
VK_DEFINE_HANDLE(VkPipeline);

namespace railguard::rendering::structs
{
    struct RenderBatch
    {
        size_t offset;
        size_t count;
        VkPipeline pipeline;
    };
} // namespace railguard::rendering::structs
