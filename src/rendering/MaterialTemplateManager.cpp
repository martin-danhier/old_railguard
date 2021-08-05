#include "../../include/rendering/MaterialTemplateManager.h"

namespace railguard::rendering
{
    void MaterialTemplateManager::Init(MaterialTemplateManagerStorage storage, size_t defaultCapacity)
    {
        super::Init(storage, defaultCapacity);

        _shaderEffects.reserve(defaultCapacity);
        _kinds.reserve(defaultCapacity);
    }

    void MaterialTemplateManager::Clear()
    {
        super::Clear();

        _shaderEffects.clear();
        _kinds.clear();
    }

    core::CompleteMatch<material_template_id_t> MaterialTemplateManager::CreateMaterialTemplate(std::vector<shader_effect_id_t> shaderEffects)
    {
        _shaderEffects.push_back(shaderEffects);

        enums::ShaderEffectKind kinds = enums::ShaderEffectKind::None;
        for (auto effectId : shaderEffects) {
            auto effect = _storage.shaderEffectManager->LookupId(effectId);
            auto effectKind = _storage.shaderEffectManager->GetEffectKind(effect);

            // Amalgamate the kinds of all available shaders in one value so we can easily know if we have a certain kind or not
            kinds = kinds | effectKind;
        }
        _kinds.push_back(kinds);

        return super::CreateItem();
    }

    void MaterialTemplateManager::DestroyMaterialTemplate(const core::Match &match)
    {
        // Get index
        auto index = match.GetIndex();
        const size_t lastIndex = _ids.size() - 1;

        // Run boilerplate deletion
        super::DestroyItem(match);

        // If the index is smaller then, the destroyed item is not the last and the last one should be moved where
        // the destroyed item was
        if (index < lastIndex)
        {
            _shaderEffects[index] = _shaderEffects[lastIndex];
            _kinds[index] = _kinds[lastIndex];
        }

        // Destroy the last item
        _shaderEffects.pop_back();
        _kinds.pop_back();
    }

    bool MaterialTemplateManager::HasEffect(const core::Match &match, shader_effect_id_t effectId) const {
        for (auto id : _shaderEffects[match.GetIndex()]) {
            if (id == effectId) {
                return true;
            }
        }

        // None found
        return false;
    }

    bool MaterialTemplateManager::HasEffectForKind(const core::Match &match, enums::ShaderEffectKind kind) const
    {
        return static_cast<bool>(_kinds[match.GetIndex()] & kind);
    }

    const std::vector<shader_effect_id_t> MaterialTemplateManager::GetShaderEffects(const core::Match &match) const
    {
        return _shaderEffects[match.GetIndex()];
    }

    const std::vector<material_template_id_t> MaterialTemplateManager::GetMaterialTemplatesWithEffectForKind(enums::ShaderEffectKind kind) const {
        std::vector<material_template_id_t> result;

        for (uint32_t i = 0; i < _kinds.size(); i++) {
            if (static_cast<bool>(_kinds[i] & kind)) {
                result.push_back(_ids[i]);
            }
        }

        return result;
    }

    const std::vector<material_template_id_t> MaterialTemplateManager::GetMaterialTemplatesWithEffects(const std::vector<shader_effect_id_t> &shaderEffectsIds) const {
        std::vector<material_template_id_t> result;
        // TODO smart thing for reserve
        result.reserve(_shaderEffects.size());

        // For each shader effect id
        for (auto shaderEffectId : shaderEffectsIds) {
            // For each template
            for (uint32_t i = 0; i < _shaderEffects.size(); i++) {
                // For each shader effect of that template
                for (auto availableEffect : _shaderEffects[i]) {
                    // If it has the requested effect
                    if (shaderEffectId == availableEffect) {
                        // Add the id of the template and stop looping in that template
                        result.push_back(_ids[i]);
                        break;
                    }
                }
            }
        }

        // This will do 2 things at once:
        // -> Find all templates that have effects from the given list
        // -> Sort the vector by shader effect, meaning all templates using the same pipeline will be next to each each other.

        // Possible side effect: if a template has several effects from the list, it will be added twice into the result.

        return result;
    }

} // namespace railguard::rendering
