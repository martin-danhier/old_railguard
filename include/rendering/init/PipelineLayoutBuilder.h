#pragma once

#include "../../includes/Vulkan.h"

namespace railguard::rendering::init
{
    /**
     * @brief Helper class that can be used to generate vk::PipelineLayout objects easily.
     */
    class PipelineLayoutBuilder
    {
    private:


    public:
        [[nodiscard]] vk::PipelineLayout Build(const vk::Device &vulkanDevice);
    };
} // namespace railguard::rendering::init
