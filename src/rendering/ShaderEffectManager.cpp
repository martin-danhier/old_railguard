#include "../../../include/rendering/ShaderEffectManager.h"
#include "../../../include/rendering/init/PipelineBuilder.h"

namespace railguard::rendering
{
    void ShaderEffectManager::Init(ShaderEffectManagerStorage storage, size_t defaultCapacity)
    {
        // Call parent function
        super::Init(storage, defaultCapacity);

        // Resize vectors that weren't initialized by parent
        _pipelineLayouts.reserve(defaultCapacity);
        _pipelines.reserve(defaultCapacity);
        _shaderStages.reserve(defaultCapacity);
    }

    void ShaderEffectManager::Clear()
    {
        super::Clear();

        // Destroy pipelines that are built
        for (vk::Pipeline pipeline : _pipelines)
        {
            if (pipeline != static_cast<vk::Pipeline>(nullptr))
            {
                _storage.vulkanDevice.destroyPipeline(pipeline);
            }
        }

        // Destroy pipeline layouts
        for (vk::PipelineLayout layout : _pipelineLayouts)
        {
            _storage.vulkanDevice.destroyPipelineLayout(layout);
        }

        // Clear vectors
        _pipelineLayouts.clear();
        _pipelines.clear();
        _shaderStages.clear();
    }

    core::CompleteMatch<shader_effect_id_t> ShaderEffectManager::CreateShaderEffect(init::ShaderEffectInitInfo initInfo, bool buildEffectAfterCreation)
    {
        auto match = super::CreateItem();

        _pipelineLayouts.push_back(initInfo.pipelineLayout);
        _shaderStages.push_back(initInfo.shaderStages);
        _pipelines.push_back(nullptr);

        // Build if we should
        if (buildEffectAfterCreation)
        {
            BuildEffect(match.ToMatch());
        }

        return match;
    }

    vk::Pipeline ShaderEffectManager::BuildEffect(const core::Match &match)
    {
        auto index = match.GetIndex();

        auto builder = init::PipelineBuilder()
            .WithPipelineLayout(_pipelineLayouts[index])
            .GetDefaultsForExtent(_storage.windowManager->GetWindowExtent());

        // Register shader stages
        for (shader_module_id_t shaderModuleId : _shaderStages[index]) {
            // Find that module
            auto match = _storage.shaderModuleManager->LookupId(shaderModuleId);
            // Register the stage
            builder.AddShaderStage(_storage.shaderModuleManager->GetStage(match), _storage.shaderModuleManager->GetModule(match));
        }

        // Build the pipeline
        vk::Pipeline pipeline = builder.Build(_storage.vulkanDevice, _storage.renderPass);

        // Store the pipeline
        _pipelines[index] = pipeline;

        return pipeline;
    }

    void ShaderEffectManager::DestroyShaderEffect(const core::Match &match) {
        super::DestroyItem(match);

        
    }


}