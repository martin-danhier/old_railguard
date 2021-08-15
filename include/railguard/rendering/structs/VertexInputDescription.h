#pragma once

#include <railguard/includes/Vulkan.h>

namespace railguard::rendering::structs
{
    /**
     * @brief Describes the bindings and attributes of the vertex input. Used to config shaders.
     *
     */
    struct VertexInputDescription
    {
        std::vector<vk::VertexInputBindingDescription> bindings{};
        std::vector<vk::VertexInputAttributeDescription> attributes{};
        vk::PipelineVertexInputStateCreateFlags flags{};
    };

} // namespace railguard::rendering::structs
