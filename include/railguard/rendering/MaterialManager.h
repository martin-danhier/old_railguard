#pragma once

#include <railguard/core/StandaloneManager.h>
#include <railguard/rendering/Ids.h>
#include <railguard/rendering/enums/ShaderEffectKind.h>

namespace railguard::rendering
{
    struct MaterialManagerStorage
    {
        const class MaterialTemplateManager *materialTemplateManager;
        const class ShaderEffectManager *shaderEffectManager;
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
        MaterialManager(MaterialManagerStorage storage, size_t defaultCapacity);
        void Clear() override;
        core::CompleteMatch<material_id_t> CreateMaterial(material_template_id_t baseTemplate);
        void DestroyMaterial(const core::Match &match);

        [[nodiscard]] material_template_id_t GetMaterialTemplate(const core::Match &match) const;
        [[nodiscard]] std::vector<model_id_t> GetModelsThatUseMaterial(uint32_t index) const;
        [[nodiscard]] std::vector<model_id_t> GetModelsThatUseMaterial(const core::Match &match) const;

        void RegisterModel(const core::Match &match, model_id_t modelId);
        void UnregisterModel(const core::Match &match, model_id_t modelId);
        void ClearModels(const core::Match &match);
        void ClearAllModels();

        [[nodiscard]] std::vector<uint32_t> GetMaterialsWhichSupportKind(enums::ShaderEffectKind kind) const;
    };
} // namespace railguard::rendering
