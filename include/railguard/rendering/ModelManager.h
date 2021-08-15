#pragma once

#include "railguard/core/StandaloneManager.h"
#include "railguard/rendering/Ids.h"

namespace railguard
{
    namespace core
    {
        struct Entity;
    }

    namespace rendering
    {
        struct ModelManagerStorage
        {
            class MaterialManager *materialManager;
        };

        class ModelManager : public core::StandaloneManager<model_id_t, ModelManagerStorage>
        {
          private:
            typedef core::StandaloneManager<model_id_t, ModelManagerStorage> super;

            std::vector<material_id_t> _materials;
            std::vector<std::vector<core::Entity>> _instances;

          public:
            ModelManager(ModelManagerStorage storage, size_t defaultCapacity);
            void Clear() override;
            core::CompleteMatch<model_id_t> CreateModel(material_id_t material);
            void DestroyModel(const core::Match &match);

            void RegisterInstance(const core::Match &match, const core::Entity &instanceEntity);
            void UnregisterInstance(const core::Match &match, const core::Entity &instanceEntity);
            void ClearInstances(const core::Match &match);
            void ClearAllInstances();

            // Getters
            [[nodiscard]] material_id_t GetMaterial(const core::Match &match) const;
            [[nodiscard]] std::vector<core::Entity> GetInstances(const core::Match &match) const;
        };

    } // namespace rendering
} // namespace railguard