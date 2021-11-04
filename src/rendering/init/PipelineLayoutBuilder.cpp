#include "railguard/rendering/init/PipelineLayoutBuilder.h"



namespace railguard::rendering::init
{
    vk::PipelineLayout PipelineLayoutBuilder::Build(const vk::Device &vulkanDevice)
    {
        // Create default create info
        vk::PipelineLayoutCreateInfo layoutCreateInfo {
            .flags                  = {},
            .setLayoutCount         = 0,
            .pSetLayouts            = nullptr,
            .pushConstantRangeCount = 0,
            .pPushConstantRanges    = nullptr,
        };

        // Changes can be added here

        return vulkanDevice.createPipelineLayout(layoutCreateInfo);
    }

} // namespace railguard::rendering::init
