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
        for (shader_module_id_t shaderModuleId : _shaderStages[index])
        {
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

    void ShaderEffectManager::DestroyShaderEffect(const core::Match &match)
    {
        super::DestroyItem(match);

        // Get index
        auto index = match.GetIndex();
        size_t lastIndex = _ids.size() - 1;

        // Destroy the pipeline if needed
        if (_pipelines[index] != static_cast<vk::Pipeline>(nullptr))
        {
            _storage.vulkanDevice.destroyPipeline(_pipelines[index]);
        }
        // Destroy the pipeline layout
        _storage.vulkanDevice.destroyPipelineLayout(_pipelineLayouts[index]);

        // If the index is smaller then, the destroyed item is not the last and the last one should be moved where
        // the destroyed item was
        if (index < lastIndex)
        {
            _pipelineLayouts[index] = _pipelineLayouts[lastIndex];
            _pipelines[index] = _pipelines[lastIndex];
            _shaderStages[index] = _shaderStages[lastIndex];
        }

        // Destroy the last item
        _pipelineLayouts.pop_back();
        _pipelines.pop_back();
        _shaderStages.pop_back();
    }

    void ShaderEffectManager::Bind(const core::Match &match, const vk::CommandBuffer &cmd) const
    {
        cmd.bindPipeline(vk::PipelineBindPoint::eGraphics, _pipelines[match.GetIndex()]);
    }

    const vk::PipelineLayout ShaderEffectManager::GetPipelineLayout(const core::Match &match) const
    {
        return _pipelineLayouts[match.GetIndex()];
    }
    const vk::Pipeline ShaderEffectManager::GetPipeline(const core::Match &match) const
    {
        return _pipelines[match.GetIndex()];
    }
    const std::vector<shader_module_id_t> ShaderEffectManager::GetShaderStages(const core::Match &match) const
    {
        return _shaderStages[match.GetIndex()];
    }

}