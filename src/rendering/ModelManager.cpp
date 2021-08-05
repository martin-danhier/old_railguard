#include "../../include/rendering/ModelManager.h"
#include <algorithm>

namespace railguard::rendering
{
    void ModelManager::Init(ModelManagerStorage storage, size_t defaultCapacity)
    {
        super::Init(storage, defaultCapacity);

        // Init vectors
        _materials.reserve(defaultCapacity);
        _instances.reserve(defaultCapacity);
    }

    void ModelManager::Clear()
    {
        super::Clear();
        _materials.clear();
        _instances.clear();

        // Signal the clearing to the material manager
        _storage.materialManager->ClearAllModels();
    }
    core::CompleteMatch<model_id_t> ModelManager::CreateModel(material_id_t material)
    {
        auto match = super::CreateItem();

        _materials.push_back(material);
        _instances.push_back(std::vector<core::Entity>(5));

        // Signal the creation to the material
        auto matMatch = _storage.materialManager->LookupId(material);
        _storage.materialManager->RegisterModel(matMatch, match.GetId());

        return match;
    }

    void ModelManager::DestroyModel(const core::Match &match)
    {

        // Get index
        const auto index = match.GetIndex();
        const size_t lastIndex = _ids.size() - 1;

        // Signal the destruction to the material
        auto matMatch = _storage.materialManager->LookupId(_materials[index]);
        _storage.materialManager->UnregisterModel(matMatch, _ids[index]);

        // Run boilerplate deletion
        super::DestroyItem(match);

        // If the index is smaller then, the destroyed item is not the last and the last one should be moved where
        // the destroyed item was
        if (index < lastIndex)
        {
            _materials[index] = _materials[lastIndex];
            _instances[index] = _instances[lastIndex];
        }

        // Destroy the last item
        _materials.pop_back();
        _instances.pop_back();
    }

    void ModelManager::RegisterInstance(const core::Match &match, const core::Entity &instanceEntity)
    {
        _instances[match.GetIndex()].push_back(instanceEntity);
    }

    void ModelManager::UnregisterInstance(const core::Match &match, const core::Entity &instanceEntity)
    {
        // Find model id in list
        auto &v = _instances[match.GetIndex()];
        // Remove value from vector
        v.erase(std::remove(v.begin(), v.end(), instanceEntity), v.end());
    }

    void ModelManager::ClearInstances(const core::Match &match)
    {
        _instances[match.GetIndex()].clear();
    }

    const std::vector<core::Entity> ModelManager::GetInstances(const core::Match &match) const {
        return _instances[match.GetIndex()];
    }

    void ModelManager::ClearAllInstances()
    {
        for (auto &v : _instances)
        {
            v.clear();
        }
    }

    // Getters
    material_id_t ModelManager::GetMaterial(const core::Match &match) const
    {
        return _materials[match.GetIndex()];
    }

} // namespace railguard::rendering
