#pragma once

#include "../../includes/Vulkan.h"
#include "../ShaderModuleManager.h"

namespace railguard::rendering::init
{
    struct ShaderEffectInitInfo {
        vk::PipelineLayout pipelineLayout;
        std::vector<shader_module_id_t> shaderStages;
    };
} // namespace railguard::rendering::init
