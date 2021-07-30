#pragma once
#include "../../includes/Vulkan.h"

namespace railguard::rendering::structs
{
    struct VertexInputDescription
    {
        std::vector<vk::VertexInputBindingDescription> bindings{};
        std::vector<vk::VertexInputAttributeDescription> attributes{};
        vk::PipelineVertexInputStateCreateFlags flags{};
    };
} // namespace railguard::rendering::structs
