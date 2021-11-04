#pragma once

#include <railguard/rendering/Ids.h>
#include <railguard/rendering/enums/ShaderEffectKind.h>
#include <vector>


#define VK_DEFINE_HANDLE(object) typedef struct object##_T *object;
VK_DEFINE_HANDLE(VkPipelineLayout)

namespace railguard::rendering::init
{
    struct ShaderEffectInitInfo
    {
        VkPipelineLayout pipelineLayout;
        std::vector<shader_module_id_t> shaderStages;
        enums::ShaderEffectKind effectKind;
    };
} // namespace railguard::rendering::init