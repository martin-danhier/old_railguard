#pragma once

#include <railguard/rendering/Ids.h>
#include <railguard/rendering/enums/ShaderEffectKind.h>
#include <railguard/rendering/structs/RenderBatch.h>
#include <vector>

namespace vk {
  class vk::CommandBuffer;
}

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
        std::vector<std::vector<material_id_t>> _materialsCache;
        std::vector<std::vector<std::vector<model_id_t>>> _modelsCache;
        std::vector<std::vector<structs::RenderBatch>> _batchesCache;
        

      public:
        RenderStageManager(const std::vector<enums::ShaderEffectKind> &stages,
                           const MaterialManager *materialManager,
                           const ModelManager *modelManager,
                           AllocationManager *allocationManager);
        ~RenderStageManager();
        void UpdateCache();
        void DrawFromCache(vk::CommandBuffer &cmd);
    };

} // namespace railguard::rendering
