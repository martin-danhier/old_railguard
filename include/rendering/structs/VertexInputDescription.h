#pragma once
#include "../../includes/Vulkan.h"

namespace railguard::rendering::structs
{
    /**
     * @brief Describes the bindings and attributes of the vertex input. Used to config shaders.
     *
     * @tparam B Number of bindings
     * @tparam A Number of attributes
     */
    template <size_t B, size_t A>
    struct VertexInputDescription
    {
        std::array<vk::VertexInputBindingDescription, B> bindings{};
        std::array<vk::VertexInputAttributeDescription, A> attributes{};
        vk::PipelineVertexInputStateCreateFlags flags{};
    };

} // namespace railguard::rendering::structs
