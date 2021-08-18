#include <algorithm>
#include <railguard/rendering/MaterialManager.h>
#include <railguard/rendering/MaterialTemplateManager.h>
#include <railguard/rendering/ShaderEffectManager.h>
#include <railguard/rendering/enums/ShaderEffectKind.h>
#include <railguard/rendering/structs/BuiltTemplate.h>
#include <railguard/rendering/structs/RenderBatch.h>

namespace railguard::rendering
{
    MaterialManager::MaterialManager(MaterialManagerStorage storage, size_t defaultCapacity) : super(storage, defaultCapacity)
    {
        // Init vectors
        _baseTemplates.reserve(defaultCapacity);
        _modelsUsingMaterial.reserve(defaultCapacity);
    }

    void MaterialManager::Clear()
    {
        super::Clear();

        _baseTemplates.clear();
        _modelsUsingMaterial.clear();
    }

    core::CompleteMatch<material_id_t> MaterialManager::CreateMaterial(material_template_id_t baseTemplate)
    {
        _baseTemplates.push_back(baseTemplate);
        _modelsUsingMaterial.emplace_back(1);

        return super::CreateItem();
    }

    void MaterialManager::DestroyMaterial(const core::Match &match)
    {
        // Get index
        auto index             = match.GetIndex();
        const size_t lastIndex = _ids.size() - 1;

        // Run boilerplate deletion
        super::DestroyItem(match);

        // If the index is smaller then, the destroyed item is not the last and the last one should be moved where
        // the destroyed item was
        if (index < lastIndex)
        {
            _baseTemplates[index]       = _baseTemplates[lastIndex];
            _modelsUsingMaterial[index] = _modelsUsingMaterial[lastIndex];
        }

        // Destroy the last item
        _baseTemplates.pop_back();
        _modelsUsingMaterial.pop_back();
    }

    size_t MaterialManager::CountModelsThatUseMaterial(const core::Match &match) const
    {
        return _modelsUsingMaterial[match.GetIndex()].size();
    }

    std::vector<structs::RenderBatch> MaterialManager::GenerateBatchesForKind(enums::ShaderEffectKind kind) const
    {
        std::vector<structs::RenderBatch> batches;

        // Get effects which support the given kind, sorted by effect so that the number of required binds is minimal
        const auto effectsWithDesiredKind = _storage.shaderEffectManager->GetEffectsOfKind(kind);
        const auto supportedTemplates     = _storage.materialTemplateManager->BuildTemplatesForEffects(effectsWithDesiredKind);

        size_t modelCounter = 0;

        // For each template
        for (auto matTemplate : supportedTemplates)
        {
            // Add the materials that use that template.
            for (uint32_t i = 0; i < _baseTemplates.size(); i++)
            {
                if (_baseTemplates[i] == matTemplate.templateId)
                {
                    size_t count = _modelsUsingMaterial[i].size();

                    // Add a batch
                    batches.push_back(structs::RenderBatch{
                        .offset = modelCounter,
                        .count = count,
                        .pipeline = matTemplate.pipeline,
                    });

                    modelCounter += count;
                }
            }
        }

        return batches;
    }

    material_template_id_t MaterialManager::GetMaterialTemplate(const core::Match &match) const
    {
        return _baseTemplates[match.GetIndex()];
    }

    std::vector<model_id_t> MaterialManager::GetModelsThatUseMaterial(const core::Match &match) const
    {
        return _modelsUsingMaterial[match.GetIndex()];
    }

    std::vector<model_id_t> MaterialManager::GetModelsThatUseMaterial(uint32_t index) const
    {
        return _modelsUsingMaterial[index];
    }


    void MaterialManager::RegisterModel(const core::Match &match, model_id_t modelId)
    {
        _modelsUsingMaterial[match.GetIndex()].push_back(modelId);
    }

    void MaterialManager::UnregisterModel(const core::Match &match, model_id_t modelId)
    {
        // Find model id in list
        auto &v = _modelsUsingMaterial[match.GetIndex()];
        // Remove value from vector
        v.erase(std::remove(v.begin(), v.end(), modelId), v.end());
    }

    void MaterialManager::ClearModels(const core::Match &match)
    {
        _modelsUsingMaterial[match.GetIndex()].clear();
    }

    void MaterialManager::ClearAllModels()
    {
        for (auto &v : _modelsUsingMaterial)
        {
            v.clear();
        }
    }

} // namespace railguard::rendering
