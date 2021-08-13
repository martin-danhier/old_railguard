#include "../../include/rendering/RenderStageManager.h"

namespace railguard::rendering
{
    void RenderStageManager::Init(const std::vector<enums::ShaderEffectKind> &stages,
                                  const MaterialManager *materialManager,
                                  const ModelManager *modelManager,
                                  AllocationManager *allocationManager)
    {
        _stages            = stages;
        _materialManager   = materialManager;
        _modelManager      = modelManager;
        _allocationManager = allocationManager;

        // Fill the indirect buffers with 0s (null buffer).
        // They will be created later
        _indirectBuffers.resize(stages.size());
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

            // Once the model and material caches are up to date, we can save draw indirect commands
            // First, check if the buffer is big enough for our needs
            // If not, we need to resize it.

            constexpr vk::BufferUsageFlags indirectBufferUsageFlags = vk::BufferUsageFlagBits::eTransferDst
                                                                      | vk::BufferUsageFlagBits::eStorageBuffer
                                                                      | vk::BufferUsageFlagBits::eIndirectBuffer;

            const auto requiredIndirectBufferSize = _modelsCache[stageIndex].size();
            auto &currentIndirectBuffer           = _indirectBuffers[stageIndex];

            // If it does not exist, create it
            if (currentIndirectBuffer.IsNull())
            {
                currentIndirectBuffer =
                    _allocationManager->CreateBuffer(requiredIndirectBufferSize, indirectBufferUsageFlags, VMA_MEMORY_USAGE_GPU_ONLY);
            }
            // If it exists but is not big enough, recreate it
            else if (currentIndirectBuffer.size < requiredIndirectBufferSize)
            {
                // Maybe register it for deletion at the end of the frame later instead on doing it directly
                _allocationManager->DestroyBuffer(currentIndirectBuffer);
                currentIndirectBuffer =
                    _allocationManager->CreateBuffer(requiredIndirectBufferSize, indirectBufferUsageFlags, VMA_MEMORY_USAGE_GPU_ONLY);
            }

            // At this point, we have an indirect buffer big enough to hold the commands we want to register
        }
    }

    void RenderStageManager::DrawFromCache()
    {
        // For each stage
        for (uint32_t stageIndex = 0; stageIndex < _stages.size(); stageIndex++)
        {
            // const auto &stageKind = _stages[stageIndex];
            // const auto &stageMaterials = _materialsCache[stageIndex];
            // const auto &stageModels = _modelsCache[stageIndex];
        }
    }

    void RenderStageManager::CleanUp()
    {
        // Destroy indirect buffers
        for (auto &buffer : _indirectBuffers)
        {
            if (buffer.IsNotNull())
            {
                _allocationManager->DestroyBuffer(buffer);
            }
        }
    }

} // namespace railguard::rendering
