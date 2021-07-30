#include "../../../include/rendering/init/PipelineBuilder.h"
#include "../../../include/utils/AdvancedCheck.h"

#ifdef USE_ADVANCED_CHECKS
// Define errors
#define NO_PIPELINE_LAYOUT_ERROR "Pipeline layout must be given to the pipeline builder."
#define NO_VIEWPORT_ERROR "Viewport must be given to the pipeline builder."
#define NO_SCISSORS_ERROR "Scissors must be given to the pipeline builder."
#endif

namespace railguard::rendering::init
{

    // ===== PIPELINE BUILDER =====

    PipelineBuilder PipelineBuilder::AddShaderStage(vk::ShaderStageFlagBits stage,
                                                    vk::ShaderModule shaderModule)
    {

        // Add a new shader stage to the vector
        _shaderStages.push_back(vk::PipelineShaderStageCreateInfo{
            .stage = stage,
            .module = shaderModule,
            // Entry function of the shader, we use main conventionally
            .pName = "main",
        });

        // Return this so it is easier to chain functions
        return *this;
    }
    PipelineBuilder PipelineBuilder::WithVertexInput(const structs::VertexInputDescription &vertexInputDescription)
    {
        // Empty for now
        _vertexInputInfo = vk::PipelineVertexInputStateCreateInfo{
            .vertexBindingDescriptionCount = static_cast<uint32_t>(vertexInputDescription.bindings.size()),
            .pVertexBindingDescriptions = vertexInputDescription.bindings.data(),
            .vertexAttributeDescriptionCount = static_cast<uint32_t>(vertexInputDescription.attributes.size()),
            .pVertexAttributeDescriptions = vertexInputDescription.attributes.data(),
        };
        _vertexInputInitialized = true;

        return *this;
    }
    PipelineBuilder PipelineBuilder::WithAssemblyTopology(vk::PrimitiveTopology topology)
    {
        _inputAssembly = vk::PipelineInputAssemblyStateCreateInfo{
            .topology = topology,
            .primitiveRestartEnable = false,
        };
        _inputAssemblyInitialized = true;

        return *this;
    }
    PipelineBuilder PipelineBuilder::WithPolygonMode(vk::PolygonMode polygonMode)
    {
        _rasterizer = vk::PipelineRasterizationStateCreateInfo{
            .depthClampEnable = false,
            // Keep the primitive in the rasterization stage
            .rasterizerDiscardEnable = false,
            .polygonMode = polygonMode,
            // No backface culling
            .cullMode = vk::CullModeFlagBits::eNone,
            .frontFace = vk::FrontFace::eCounterClockwise,
            // No depth bias
            .depthBiasEnable = false,
            .depthBiasConstantFactor = 0.0f,
            .depthBiasClamp = 0.0f,
            .depthBiasSlopeFactor = 0.0f,
            // Width of the line
            .lineWidth = 1.0f,
        };
        _rasterizerInitialized = true;

        return *this;
    }
    PipelineBuilder PipelineBuilder::WithPipelineLayout(vk::PipelineLayout pipelineLayout)
    {
        _pipelineLayout = pipelineLayout;
#ifdef USE_ADVANCED_CHECKS
        _pipelineLayoutInitialized = true;
#endif

        return *this;
    }

    // Scissors

    PipelineBuilder PipelineBuilder::WithScissors(int32_t xOffset, int32_t yOffset, vk::Extent2D extent)
    {
        _scissor = vk::Rect2D{
            .offset{
                .x = xOffset,
                .y = yOffset,
            },
            .extent = extent,
        };
#ifdef USE_ADVANCED_CHECKS
        _scissorsInitialized = true;
#endif
        return *this;
    }

    PipelineBuilder PipelineBuilder::WithScissors(vk::Rect2D scissors)
    {
        _scissor = scissors;
#ifdef USE_ADVANCED_CHECKS
        _scissorsInitialized = true;
#endif
        return *this;
    }

    PipelineBuilder PipelineBuilder::WithDepthTestingSettings(bool doDepthTest, bool doDepthWrite,
                                                              vk::CompareOp compareOp)
    {

        _depthStencilCreateInfo = vk::PipelineDepthStencilStateCreateInfo{
            .depthTestEnable = doDepthTest,
            .depthWriteEnable = doDepthWrite,
            .depthCompareOp = doDepthTest ? compareOp : vk::CompareOp::eAlways,
            .depthBoundsTestEnable = false,
            .stencilTestEnable = false,
            .minDepthBounds = 0.0f,
            .maxDepthBounds = 1.0f,
        };

        _depthSettingsProvided = true;
        return *this;
    }

    // Viewport

    PipelineBuilder PipelineBuilder::WithViewport(float x, float y, float width, float height, float minDepth,
                                                  float maxDepth)
    {

        _viewport = vk::Viewport{
            .x = x,
            .y = y,
            .width = width,
            .height = height,
            .minDepth = minDepth,
            .maxDepth = maxDepth,
        };
#ifdef USE_ADVANCED_CHECKS
        _viewportInitialized = true;
#endif
        return *this;
    }

    PipelineBuilder PipelineBuilder::WithViewport(vk::Viewport viewport)
    {
        _viewport = viewport;
#ifdef USE_ADVANCED_CHECKS
        _viewportInitialized = true;
#endif
        return *this;
    }
    // Defaults
    PipelineBuilder PipelineBuilder::GetDefaultsForExtent(vk::Extent2D windowExtent)
    {
        // Setup a viewport that takes the whole screen
        WithViewport(0.0f, 0.0f, static_cast<float_t>(windowExtent.width), static_cast<float_t>(windowExtent.height), 0.0f, 1.0f);
        WithScissors(0, 0, windowExtent);

        return *this;
    }

    vk::Pipeline PipelineBuilder::Build(vk::Device device, vk::RenderPass pass)
    {

        // Set pipeline blend
        _colorBlendAttachment = vk::PipelineColorBlendAttachmentState{
            .blendEnable = false,
            .colorWriteMask = vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG |
                              vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA,
        };

        // Set pipeline layout to default
        _multisampling = vk::PipelineMultisampleStateCreateInfo{
            .rasterizationSamples = vk::SampleCountFlagBits::e1,
            .sampleShadingEnable = false,
            .minSampleShading = 1.0f,
            .pSampleMask = nullptr,
            .alphaToCoverageEnable = false,
            .alphaToOneEnable = false,
        };

        // Apply defaults if needed
        if (!_rasterizerInitialized)
            WithPolygonMode(vk::PolygonMode::eFill);
        if (!_inputAssemblyInitialized)
            WithAssemblyTopology(vk::PrimitiveTopology::eTriangleList);
        if (!_vertexInputInitialized)
            WithVertexInput(structs::VertexInputDescription{
                .bindings = {},
                .attributes = {},
            });
        if (!_depthSettingsProvided)
            WithDepthTestingSettings(false, false);

        // Create viewport state from stored viewport and scissors
        vk::PipelineViewportStateCreateInfo viewportState{
            .viewportCount = 1,
            .pViewports = &_viewport,
            .scissorCount = 1,
            .pScissors = &_scissor,
        };
        // Create color blending state
        vk::PipelineColorBlendStateCreateInfo colorBlending{
            .logicOpEnable = false,
            .logicOp = vk::LogicOp::eCopy,
            .attachmentCount = 1,
            .pAttachments = &_colorBlendAttachment,
        };

        // If enabled, check that the required elements were given to the builder
        ADVANCED_CHECK(_pipelineLayoutInitialized, NO_PIPELINE_LAYOUT_ERROR);
        ADVANCED_CHECK(_viewportInitialized, NO_VIEWPORT_ERROR);
        ADVANCED_CHECK(_scissorsInitialized, NO_SCISSORS_ERROR);

        // Create the pipeline
        vk::GraphicsPipelineCreateInfo pipelineCreateInfo{
            .stageCount = static_cast<uint32_t>(_shaderStages.size()),
            .pStages = _shaderStages.data(),
            .pVertexInputState = &_vertexInputInfo,
            .pInputAssemblyState = &_inputAssembly,
            .pViewportState = &viewportState,
            .pRasterizationState = &_rasterizer,
            .pMultisampleState = &_multisampling,
            .pDepthStencilState = &_depthStencilCreateInfo,
            .pColorBlendState = &colorBlending,
            .layout = _pipelineLayout,
            .renderPass = pass,
            .subpass = 0,
            .basePipelineHandle = nullptr,
        };

        auto result = device.createGraphicsPipeline(nullptr, pipelineCreateInfo);
        // Handle result
        switch (result.result)
        {
        case vk::Result::eSuccess:
            return result.value;
        // Default returns an exception
        default:
            throw std::runtime_error("Failed to create Pipeline");
        }
    }

} // namespace railguard::rendering::init
