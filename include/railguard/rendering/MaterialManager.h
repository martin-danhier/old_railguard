#pragma once

#include "../core/StandaloneManager.h"
#include "Ids.h"
#include "MaterialTemplateManager.h"

namespace railguard::rendering
{
    struct MaterialManagerStorage
    {
        const MaterialTemplateManager *materialTemplateManager;
        const ShaderEffectManager *shaderEffectManager;
    };

    class MaterialManager : public core::StandaloneManager<material_id_t, MaterialManagerStorage>
    {
      private:
        // Typedef the parent type to make it easier to call from the methods
        typedef core::StandaloneManager<material_id_t, MaterialManagerStorage> super;

        /**
         * @brief A material derives from a template.
         */
        std::vector<material_template_id_t> _baseTemplates;
        std::vector<std::vector<model_id_t>> _modelsUsingMaterial;

        // Will contain textures later TODO
      public:
        void Init(MaterialManagerStorage storage, size_t defaultCapacity = 10);
        void Clear();
        core::CompleteMatch<material_id_t> CreateMaterial(material_template_id_t baseTemplate);
        void DestroyMaterial(const core::Match &match);

        material_template_id_t GetMaterialTemplate(const core::Match &match) const;
        const std::vector<model_id_t> GetModelsThatUseMaterial(uint32_t index) const;
        const std::vector<model_id_t> GetModelsThatUseMaterial(const core::Match &match) const;

        void RegisterModel(const core::Match &match, model_id_t modelId);
        void UnregisterModel(const core::Match &match, model_id_t modelId);
        void ClearModels(const core::Match &match);
        void ClearAllModels();

        const std::vector<uint32_t> GetMaterialsWhichSupportKind(enums::ShaderEffectKind kind) const;
    };
} // namespace railguard::rendering
