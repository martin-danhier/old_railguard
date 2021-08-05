#include "../../include/rendering/RenderStageManager.h"

namespace railguard::rendering
{

    void RenderStageManager::Init(std::vector<enums::ShaderEffectKind> stages, const MaterialManager *materialManager, const ModelManager *modelManager)
    {
        _stages = stages;
        _materialManager = materialManager;
        _modelManager = modelManager;
    }

    void RenderStageManager::UpdateCache()
    {
        // Clear current cache
        _materialsCache.clear();
        _modelsCache.clear();

        // For each stage
        for (uint32_t stageIndex = 0; stageIndex < _stages.size(); stageIndex++)
        {
            const auto &stageKind = _stages[stageIndex];

            // Get the materials that support the shader kind of that stage.
            // These materials are returned sorted by material template, which are sorted by shader effect
            // That means that the pipelines will be bound a minimum of times
            _materialsCache.push_back(_materialManager->GetMaterialsWhichSupportKind(stageKind));

            // For each material
            std::vector<std::vector<model_id_t>> modelCacheForThisStage;
            modelCacheForThisStage.reserve(_materialsCache[stageIndex].size());

            for (uint32_t materialIndex : _materialsCache[stageIndex])
            {
                const std::vector<model_id_t> modelsWithMaterials = _materialManager->GetModelsThatUseMaterial(materialIndex);
                modelCacheForThisStage.push_back(modelsWithMaterials);

                // For each model
                // for (auto model : modelsWithMaterials)
                // {
                //     const auto modelMatch = _modelManager->LookupId(model);

                // Get instances of that model
                // const std::vector<core::Entity> instances = _modelManager->GetInstances(modelMatch);

                // }
            }

            _modelsCache.push_back(modelCacheForThisStage);
        }
    }

    void RenderStageManager::DrawFromCache()
    {
        // For each stage
        for (uint32_t stageIndex = 0; stageIndex < _stages.size(); stageIndex++)
        {
            const auto &stageKind = _stages[stageIndex];
            const auto &stageMaterials = _materialsCache[stageIndex];
            const auto &stageModels = _modelsCache[stageIndex];

            

        }
    }
} // namespace railguard::rendering
