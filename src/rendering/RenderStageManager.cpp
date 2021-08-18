#include <railguard/includes/Vulkan.h>
#include <railguard/rendering/AllocationManager.h>
#include <railguard/rendering/MaterialManager.h>
#include <railguard/rendering/ModelManager.h>
#include <railguard/rendering/RenderStageManager.h>
#include <railguard/rendering/enums/ShaderEffectKind.h>
#include <railguard/rendering/structs/AllocatedStructs.h>

namespace railguard::rendering
{

    RenderStageManager::RenderStageManager(const std::vector<enums::ShaderEffectKind> &stages,
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
        _batchesCache.clear();

        // For each stage
        for (uint32_t stageIndex = 0; stageIndex < _stages.size(); stageIndex++)
        {
            const enums::ShaderEffectKind &stageKind = _stages[stageIndex];

            // ==== WORK IN PROGRESS ====

            // Get the materials that support the shader kind of that stage.
            // These materials are returned sorted by material template, which are sorted by shader effect
            // That means that the pipelines will be bound a minimum of times
            _batchesCache.push_back(_materialManager->GenerateBatchesForKind(stageKind));
            const auto &batches = _batchesCache[stageIndex];

            const auto totalCount = batches[batches.size() - 1].count + batches[batches.size() - 1].offset;

            // Prepare cache
//            std::vector<std::vector<model_id_t>> modelCacheForThisStage;
//            modelCacheForThisStage.reserve(_materialsCache[stageIndex].size());
//
//            std::vector<structs::RenderBatch> stageBatches;
//            stageBatches.reserve(_materialsCache[stageIndex].size());

//            for (uint32_t materialIndex : _materialsCache[stageIndex])
//            {
//                const std::vector<model_id_t> modelsWithMaterials = _materialManager->GetModelsThatUseMaterial(materialIndex);
//                modelCacheForThisStage.push_back(modelsWithMaterials);

                // For each model
                // for (auto model : modelsWithMaterials)
                // {
                //     const auto modelMatch = _modelManager->LookupId(model);

                // Get instances of that model
                // const std::vector<core::Entity> instances = _modelManager->GetInstances(modelMatch);

                // }
//            }

//            _modelsCache.push_back(modelCacheForThisStage);

            // Once the model and material caches are up to date, we can save draw indirect commands
            // First, check if the buffer is big enough for our needs
            // If not, we need to resize it.

            constexpr vk::BufferUsageFlags indirectBufferUsageFlags = vk::BufferUsageFlagBits::eTransferDst
                                                                      | vk::BufferUsageFlagBits::eStorageBuffer
                                                                      | vk::BufferUsageFlagBits::eIndirectBuffer;
            constexpr VmaMemoryUsage indirectBufferMemoryUsage = VMA_MEMORY_USAGE_CPU_TO_GPU;

            const auto requiredIndirectBufferSize = totalCount * sizeof(vk::DrawIndirectCommand);
            auto &currentIndirectBuffer           = _indirectBuffers[stageIndex];

            // If it does not exist, create it
            if (currentIndirectBuffer.IsNull())
            {
                currentIndirectBuffer =
                    _allocationManager->CreateBuffer(requiredIndirectBufferSize, indirectBufferUsageFlags, indirectBufferMemoryUsage);
            }
            // If it exists but is not big enough, recreate it
            else if (currentIndirectBuffer.size < requiredIndirectBufferSize)
            {
                // Maybe register it for deletion at the end of the frame later instead on doing it directly
                _allocationManager->DestroyBuffer(currentIndirectBuffer);
                currentIndirectBuffer =
                    _allocationManager->CreateBuffer(requiredIndirectBufferSize, indirectBufferUsageFlags, indirectBufferMemoryUsage);
            }

            // At this point, we have an indirect buffer big enough to hold the commands we want to register

            // Register commands
            auto drawCommands = static_cast<vk::DrawIndirectCommand *>(_allocationManager->MapBuffer(currentIndirectBuffer));
            for (size_t i = 0; i < 1; i++) // WIP hardcoded pour que ça marche mais WIP quand meme
            {
                drawCommands[i].vertexCount   = 3;
                drawCommands[i].instanceCount = 1;
                drawCommands[i].firstVertex   = 0;
                drawCommands[i].firstInstance = 0;
            }
            _allocationManager->UnmapBuffer(currentIndirectBuffer);
        }
    }

    void RenderStageManager::DrawFromCache(vk::CommandBuffer &cmd)
    {
        // For each stage
        for (uint32_t stageIndex = 0; stageIndex < _stages.size(); stageIndex++)
        {
            // const auto &stageKind = _stages[stageIndex];
//            const auto &stageMaterials = _materialsCache[stageIndex];
            // const auto &stageModels    = _modelsCache[stageIndex];

            // Get indirect buffer
//            auto &currentIndirectBuffer = _indirectBuffers[stageIndex];
//            uint32_t drawStride         = sizeof(vk::DrawIndirectCommand);
//
//            uint32_t drawnModels = 0;
//
//            for (size_t i = 0; i < stageMaterials.size(); i++)
//            {
//                // For now, we make one draw call per material (we will bind descriptor sets)
//                // Maybe later we will cache all material sets globally and we will be able to only stop for each shader effect
//                auto mat        = _materialManager->LookupId(stageMaterials[i]);
//                auto modelCount = _materialManager->CountModelsThatUseMaterial(mat); // TODO cache that
//
//                // TODO -> cache template etc so that here we just check and bind. We don't want to move outside of the
//                // RenderStageManager here
//
//                vk::DeviceSize indirectOffset = drawnModels * sizeof(vk::DrawIndirectCommand);
//
//                // Draw indirect
//                cmd.drawIndirect(currentIndirectBuffer.buffer, indirectOffset, modelCount, drawStride);
//
//                drawnModels += modelCount;
//            }
        }
    }

    RenderStageManager::~RenderStageManager()
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
