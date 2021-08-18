#pragma once

#include <railguard/rendering/Ids.h>
#include <railguard/includes/Vulkan.h>

namespace railguard::rendering::structs
{
    struct BuiltTemplate {
        material_template_id_t templateId;
        vk::Pipeline pipeline;
    };
} // namespace railguard::rendering::structs
