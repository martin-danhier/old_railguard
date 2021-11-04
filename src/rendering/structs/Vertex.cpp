
#include <railguard/rendering/structs/Vertex.h>
#include <railguard/rendering/structs/VertexInputDescription.h>

namespace railguard::rendering::structs
{
    VertexInputDescription Vertex::GetVertexDescription()
    {
        return VertexInputDescription {
            // Bindings
            .bindings =
                {
                    vk::VertexInputBindingDescription {
                        .binding   = 0,
                        .stride    = sizeof(Vertex),
                        .inputRate = vk::VertexInputRate::eVertex,
                    },
                },
            // Attributes
            .attributes =
                {
                    // location 0 : position
                    vk::VertexInputAttributeDescription {
                        .location = 0,
                        .binding  = 0,
                        .format   = vk::Format::eR32G32B32Sfloat,
                        .offset   = static_cast<uint32_t>(offsetof(Vertex, position)),
                    },
                    // location 1 : normal
                    vk::VertexInputAttributeDescription {
                        .location = 1,
                        .binding  = 0,
                        .format   = vk::Format::eR32G32B32Sfloat,
                        .offset   = static_cast<uint32_t>(offsetof(Vertex, normal)),
                    },
                    // location 2 : uv
                    vk::VertexInputAttributeDescription {
                        .location = 2,
                        .binding  = 0,
                        .format   = vk::Format::eR32G32Sfloat,
                        .offset   = static_cast<uint32_t>(offsetof(Vertex, uv)),
                    },
                },
            // Flags
            .flags = {},
        };
    }
} // namespace railguard::rendering::structs
