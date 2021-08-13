#pragma once

#include "ModelManager.h"
#include "AllocationManager.h"

namespace railguard::rendering
{

    class RenderStageManager
    {
    private:
        // Pointer to other managers
        const MaterialManager *_materialManager;
        const ModelManager *_modelManager;
        AllocationManager *_allocationManager;

        // Stages to render
        std::vector<enums::ShaderEffectKind> _stages;
        std::vector<structs::AllocatedBuffer> _indirectBuffers;

        // Cache
        std::vector<std::vector<material_id_t>> _materialsCache;
        std::vector<std::vector<std::vector<model_id_t>>> _modelsCache;

    public:
        void Init(const std::vector<enums::ShaderEffectKind>& stages, const MaterialManager *materialManager, const ModelManager *modelManager, AllocationManager *allocationManager);
        void UpdateCache();
        void DrawFromCache();
        void CleanUp();
    };

} // namespace railguard::rendering
