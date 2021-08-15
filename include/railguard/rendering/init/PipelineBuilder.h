#pragma once

#include "railguard/includes/Vulkan.h"

namespace railguard::rendering
{
    namespace structs {
        struct VertexInputDescription;
    }
    namespace init
    {
        class PipelineBuilder
        {
          private:
            std::vector<vk::PipelineShaderStageCreateInfo> _shaderStages;
            vk::PipelineVertexInputStateCreateInfo _vertexInputInfo;
            vk::PipelineInputAssemblyStateCreateInfo _inputAssembly;
            vk::Viewport _viewport;
            vk::Rect2D _scissor;
            vk::PipelineRasterizationStateCreateInfo _rasterizer;
            vk::PipelineColorBlendAttachmentState _colorBlendAttachment;
            vk::PipelineMultisampleStateCreateInfo _multisampling;
            vk::PipelineLayout _pipelineLayout;
            vk::PipelineDepthStencilStateCreateInfo _depthStencilCreateInfo;

            // Booleans to store whether default should be applied or not
            bool _rasterizerInitialized    = false;
            bool _inputAssemblyInitialized = false;
            bool _vertexInputInitialized   = false;
            bool _depthSettingsProvided    = false;
#ifdef USE_ADVANCED_CHECKS
            bool _pipelineLayoutInitialized = false;
            bool _scissorsInitialized       = false;
            bool _viewportInitialized       = false;
#endif

          public:
            PipelineBuilder AddShaderStage(vk::ShaderStageFlagBits stage, vk::ShaderModule shaderModule);
            PipelineBuilder WithVertexInput(const structs::VertexInputDescription &vertexInputDescription);
            PipelineBuilder WithAssemblyTopology(vk::PrimitiveTopology topology);
            PipelineBuilder WithPolygonMode(vk::PolygonMode polygonMode);
            PipelineBuilder WithPipelineLayout(vk::PipelineLayout pipelineLayout);
            PipelineBuilder WithScissors(int32_t xOffset, int32_t yOffset, vk::Extent2D extent);
            PipelineBuilder WithScissors(vk::Rect2D scissors);
            PipelineBuilder
                WithDepthTestingSettings(bool doDepthTest, bool doDepthWrite, vk::CompareOp compareOp = vk::CompareOp::eAlways);
            PipelineBuilder WithViewport(float x, float y, float width, float height, float minDepth, float maxDepth);
            PipelineBuilder WithViewport(vk::Viewport viewport);
            PipelineBuilder GetDefaultsForExtent(vk::Extent2D windowExtent);
            vk::Pipeline Build(vk::Device device, vk::RenderPass pass);
        };
    } // namespace init
} // namespace railguard::rendering