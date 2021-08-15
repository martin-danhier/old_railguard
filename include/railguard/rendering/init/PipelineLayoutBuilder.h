#pragma once

namespace vk {
    class Device;
    class PipelineLayout;
}

namespace railguard::rendering::init
{
    /**
     * @brief Helper class that can be used to generate vk::PipelineLayout objects easily.
     */
    class PipelineLayoutBuilder
    {
    private:


    public:
        [[nodiscard]] static vk::PipelineLayout Build(const vk::Device &vulkanDevice);
    };
} // namespace railguard::rendering::init
