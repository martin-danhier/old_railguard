#pragma once

#include <railguard/core/StandaloneManager.h>
#include <railguard/rendering/Ids.h>
#include <railguard/rendering/enums/ShaderEffectKind.h>

namespace railguard::rendering
{
    struct MaterialTemplateManagerStorage
    {
        const class ShaderEffectManager *shaderEffectManager;
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
        MaterialTemplateManager(MaterialTemplateManagerStorage storage, size_t defaultCapacity);
        void Clear() override;
        core::CompleteMatch<material_template_id_t> CreateMaterialTemplate(const std::vector<shader_effect_id_t>& shaderEffects);
        void DestroyMaterialTemplate(const core::Match &match);
        [[nodiscard]] bool HasEffect(const core::Match &match, shader_effect_id_t effectId) const;
        [[nodiscard]] bool HasEffectForKind(const core::Match &match, enums::ShaderEffectKind kind) const;
        [[nodiscard]] std::vector<material_template_id_t> GetMaterialTemplatesWithEffectForKind(enums::ShaderEffectKind kind) const;
        [[nodiscard]] std::vector<material_template_id_t> GetMaterialTemplatesWithEffects(const std::vector<shader_effect_id_t> &shaderEffectsIds) const;

        // Getters
        [[nodiscard]] std::vector<shader_effect_id_t> GetShaderEffects(const core::Match &match) const;
    };
} // namespace railguard::rendering
