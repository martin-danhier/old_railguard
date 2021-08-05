#pragma once

#include "../core/StandaloneManager.h"
#include "ShaderEffectManager.h"
#include "Ids.h"

namespace railguard::rendering
{
    struct MaterialTemplateManagerStorage
    {
        const ShaderEffectManager *shaderEffectManager;
    };

    class MaterialTemplateManager : public core::StandaloneManager<material_template_id_t, MaterialTemplateManagerStorage>
    {
    private:
        // Typedef the parent type to make it easier to call from the methods
        typedef core::StandaloneManager<material_template_id_t, MaterialTemplateManagerStorage> super;

        /**
         * @brief Shader effects linked to this material template.
         */
        std::vector<std::vector<shader_effect_id_t>> _shaderEffects;
        /**
         * @brief Flag amalgamating the kinds of all linked shader effects. This allows to quickly check whether a given kind is supported or not.
         */
        std::vector<enums::ShaderEffectKind> _kinds;

    public:
        void Init(MaterialTemplateManagerStorage storage, size_t defaultCapacity = 3);
        void Clear();
        core::CompleteMatch<material_template_id_t> CreateMaterialTemplate(std::vector<shader_effect_id_t> shaderEffects);
        void DestroyMaterialTemplate(const core::Match &match);
        bool HasEffect(const core::Match &match, shader_effect_id_t effectId) const;
        bool HasEffectForKind(const core::Match &match, enums::ShaderEffectKind kind) const;
        const std::vector<material_template_id_t> GetMaterialTemplatesWithEffectForKind(enums::ShaderEffectKind kind) const;
        const std::vector<material_template_id_t> GetMaterialTemplatesWithEffects(const std::vector<shader_effect_id_t> &shaderEffectsIds) const;

        // Getters
        const std::vector<shader_effect_id_t> GetShaderEffects(const core::Match &match) const;
    };
} // namespace railguard::rendering
