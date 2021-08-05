#pragma once

#include "../../includes/Vulkan.h"
#include "../ShaderModuleManager.h"
#include "../enums/ShaderEffectKind.h"

namespace railguard::rendering::init
{
    struct ShaderEffectInitInfo {
        vk::PipelineLayout pipelineLayout;
        std::vector<shader_module_id_t> shaderStages;
        enums::ShaderEffectKind effectKind;
    };
} // namespace railguard::rendering::init
