#pragma once

#include "../core/StandaloneManager.h"
#include "../core/EntityManager.h"
#include "MaterialManager.h"

namespace railguard::rendering
{
    struct ModelManagerStorage
    {
        MaterialManager *materialManager;
    };

    class ModelManager : public core::StandaloneManager<model_id_t, ModelManagerStorage>
    {
    private:
        typedef core::StandaloneManager<model_id_t, ModelManagerStorage> super;

        std::vector<material_id_t> _materials;
        std::vector<std::vector<core::Entity>> _instances;

    public:
        void Init(ModelManagerStorage storage, size_t defaultCapacity = 30);
        void Clear();
        core::CompleteMatch<model_id_t> CreateModel(material_id_t material);
        void DestroyModel(const core::Match &match);

        void RegisterInstance(const core::Match &match, const core::Entity &instanceEntity);
        void UnregisterInstance(const core::Match &match, const core::Entity &instanceEntity);
        void ClearInstances(const core::Match &match);
        void ClearAllInstances();

        // Getters
        material_id_t GetMaterial(const core::Match &match) const;
        const std::vector<core::Entity> GetInstances(const core::Match &match) const;
    };

} // namespace railguard::rendering
