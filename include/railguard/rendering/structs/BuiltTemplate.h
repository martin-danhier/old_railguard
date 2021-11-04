#pragma once

#include <railguard/rendering/Ids.h>

#define VK_DEFINE_HANDLE(object) typedef struct object##_T* object;
VK_DEFINE_HANDLE(VkPipeline)

namespace railguard::rendering::structs
{
    struct BuiltTemplate {
        material_template_id_t templateId;
        VkPipeline pipeline;
    };
} // namespace railguard::rendering::structs
