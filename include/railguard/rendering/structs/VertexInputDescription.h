#pragma once

#include <vector>

struct VkVertexInputBindingDescription;
struct VkVertexInputAttributeDescription;
typedef uint32_t VkFlags;
typedef VkFlags VkPipelineVertexInputStateCreateFlags;

namespace railguard::rendering::structs
{
    /**
     * @brief Describes the bindings and attributes of the vertex input. Used to config shaders.
     *
     */
    struct VertexInputDescription
    {
        std::vector<VkVertexInputBindingDescription> bindings {};
        std::vector<VkVertexInputAttributeDescription> attributes {};
        VkPipelineVertexInputStateCreateFlags flags {};
    };

} // namespace railguard::rendering::structs
