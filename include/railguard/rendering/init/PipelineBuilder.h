#pragma once

#include <vector>
#include <vulkan/vulkan.h>

namespace railguard::rendering
{
    namespace structs
    {
        struct VertexInputDescription;
    }
    namespace init
    {
        class PipelineBuilder
        {
          private:
            std::vector<VkPipelineShaderStageCreateInfo> _shaderStages;
            VkPipelineVertexInputStateCreateInfo _vertexInputInfo;
            VkPipelineInputAssemblyStateCreateInfo _inputAssembly;
            VkViewport _viewport;
            VkRect2D _scissor;
            VkPipelineRasterizationStateCreateInfo _rasterizer;
            VkPipelineColorBlendAttachmentState _colorBlendAttachment;
            VkPipelineMultisampleStateCreateInfo _multisampling;
            VkPipelineLayout _pipelineLayout;
            VkPipelineDepthStencilStateCreateInfo _depthStencilCreateInfo;

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
            PipelineBuilder AddShaderStage(VkShaderStageFlagBits stage, VkShaderModule shaderModule);
            PipelineBuilder WithVertexInput(const structs::VertexInputDescription &vertexInputDescription);
            PipelineBuilder WithAssemblyTopology(VkPrimitiveTopology topology);
            PipelineBuilder WithPolygonMode(VkPolygonMode polygonMode);
            PipelineBuilder WithPipelineLayout(VkPipelineLayout pipelineLayout);
            PipelineBuilder WithScissors(int32_t xOffset, int32_t yOffset, VkExtent2D extent);
            PipelineBuilder WithScissors(VkRect2D scissors);
            PipelineBuilder
                WithDepthTestingSettings(bool doDepthTest, bool doDepthWrite, VkCompareOp compareOp = VK_COMPARE_OP_ALWAYS);
            PipelineBuilder WithViewport(float x, float y, float width, float height, float minDepth, float maxDepth);
            PipelineBuilder WithViewport(VkViewport viewport);
            PipelineBuilder GetDefaultsForExtent(VkExtent2D windowExtent);
            VkPipeline Build(VkDevice device, VkRenderPass pass);
        };
    } // namespace init
} // namespace railguard::rendering