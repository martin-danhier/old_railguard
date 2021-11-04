#pragma once

#include <railguard/rendering/Ids.h>
#include <railguard/rendering/enums/ShaderEffectKind.h>
#include <railguard/rendering/structs/RenderBatch.h>
#include <vector>

#define VK_DEFINE_HANDLE(object) typedef struct object##_T* object;
VK_DEFINE_HANDLE(VkCommandBuffer)

namespace railguard::rendering
{
    namespace structs
    {
        struct AllocatedBuffer;
    }

    class RenderStageManager
    {
      private:
        // Pointer to other managers
        const class MaterialManager *_materialManager;
        const class ModelManager *_modelManager;
        class AllocationManager *_allocationManager;

        // Stages to render
        std::vector<enums::ShaderEffectKind> _stages;
        std::vector<structs::AllocatedBuffer> _indirectBuffers;

        // Cache
        std::vector<std::vector<structs::RenderBatch>> _batchesCache;
        

      public:
        RenderStageManager(const std::vector<enums::ShaderEffectKind> &stages,
                           const MaterialManager *materialManager,
                           const ModelManager *modelManager,
                           AllocationManager *allocationManager);
        ~RenderStageManager();
        void UpdateCache();
        void DrawFromCache(VkCommandBuffer cmd);
    };

} // namespace railguard::rendering
