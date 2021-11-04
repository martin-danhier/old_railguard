
#include <railguard/rendering/AllocationManager.h>
#include <railguard/rendering/MaterialManager.h>
#include <railguard/rendering/ModelManager.h>
#include <railguard/rendering/RenderStageManager.h>
#include <railguard/rendering/enums/ShaderEffectKind.h>
#include <railguard/rendering/structs/AllocatedStructs.h>
#include <railguard/rendering/structs/BatchGenerationResult.h>
#include <railguard/rendering/structs/RenderBatch.h>

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
        _batchesCache.clear();

        // For each stage
        for (uint32_t stageIndex = 0; stageIndex < _stages.size(); stageIndex++)
        {
            const enums::ShaderEffectKind &stageKind = _stages[stageIndex];

            // ==== WORK IN PROGRESS ====

            // Get the materials that support the shader kind of that stage.
            // These materials are returned sorted by material template, which are sorted by shader effect
            // That means that the pipelines will be bound a minimum of times
            auto generationResult = _materialManager->GenerateBatchesForKind(stageKind);
            _batchesCache.push_back(generationResult.batches); // should we std::move ?

            const auto &batches = _batchesCache[stageIndex];
            const auto &models  = generationResult.models;

            

            // Prepare draw indirect commands
            // First, check if the buffer is big enough for our needs
            // If not, we need to resize it.

            constexpr VkBufferUsageFlags indirectBufferUsageFlags = VkBufferUsageFlagBits::eTransferDst
                                                                      | VkBufferUsageFlagBits::eStorageBuffer
                                                                      | VkBufferUsageFlagBits::eIndirectBuffer;
            constexpr VmaMemoryUsage indirectBufferMemoryUsage = VMA_MEMORY_USAGE_CPU_TO_GPU;

            const auto requiredIndirectBufferSize = models.size() * sizeof(VkDrawIndirectCommand);
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
            auto drawCommands = static_cast<VkDrawIndirectCommand *>(_allocationManager->MapBuffer(currentIndirectBuffer));
            for (size_t i = 0; i < models.size(); i++)
            {
                drawCommands[i].vertexCount   = 3; // TODO when mesh is added
                drawCommands[i].instanceCount = 1; // TODO when instances are added
                drawCommands[i].firstVertex   = 0;
                drawCommands[i].firstInstance = 0;
            }
            _allocationManager->UnmapBuffer(currentIndirectBuffer);
        }
    }

    void RenderStageManager::DrawFromCache(VkCommandBuffer cmd)
    {
        // For each stage
        for (uint32_t stageIndex = 0; stageIndex < _stages.size(); stageIndex++)
        {
            const auto &batches = _batchesCache[stageIndex];

            // Get indirect buffer
            auto &currentIndirectBuffer = _indirectBuffers[stageIndex];
            uint32_t drawStride         = sizeof(VkDrawIndirectCommand);

            VkPipeline boundPipeline = nullptr;

            for (auto &batch : batches) {
                
                // Bind pipeline if needed
                if (batch.pipeline != boundPipeline) {
                    cmd.bindPipeline(VkPipelineBindPoint::eGraphics, batch.pipeline);
                    boundPipeline = batch.pipeline;
                }

                // Draw batch of commands
                uint32_t drawOffset = sizeof(VkDrawIndirectCommand) * batch.offset;
                cmd.drawIndirect(currentIndirectBuffer.buffer, drawOffset, batch.count, drawStride);
            }

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
