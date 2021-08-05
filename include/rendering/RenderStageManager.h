#pragma once

#include "ModelManager.h"

namespace railguard::rendering
{

    class RenderStageManager
    {
    private:
        // Pointer to other managers
        const MaterialManager *_materialManager;
        const ModelManager *_modelManager;

        // Stages to render
        std::vector<enums::ShaderEffectKind> _stages;

        // Cache
        std::vector<std::vector<material_id_t>> _materialsCache;
        std::vector<std::vector<std::vector<model_id_t>>> _modelsCache;

    public:
        void Init(std::vector<enums::ShaderEffectKind> stages, const MaterialManager *materialManager, const ModelManager *modelManager);
        void UpdateCache();
        void DrawFromCache();
    };

} // namespace railguard::rendering
