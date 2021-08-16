#include <railguard/rendering/ModelManager.h>
#include <railguard/rendering/RenderNodeManager.h>
#include <vulkan/vulkan.h>

namespace railguard::rendering
{
    RenderNodeManager::RenderNodeManager(RenderNodeManagerStorage storage, const size_t defaultComponentCapacity)
        : super(storage, defaultComponentCapacity)
    {
        _models.reserve(defaultComponentCapacity);
        _transforms.reserve(defaultComponentCapacity);
    }

    void RenderNodeManager::Clear()
    {
        super::Clear();

        // Signal clearing to model manager
        _storage.modelManager->ClearAllInstances();

        _models.clear();
        _transforms.clear();
    }

    core::Match RenderNodeManager::CreateComponent(const core::Entity &entity, model_id_t model)
    {
        // Add the model
        _models.push_back(model);
        _transforms.push_back(glm::mat4()); // TODO

        // Notify the model
        auto modelMatch = _storage.modelManager->LookupId(model);
        _storage.modelManager->RegisterInstance(modelMatch, entity);

        return super::RegisterComponent(entity);
    }

    void RenderNodeManager::DestroyComponent(const core::Match &match)
    {
        auto index     = match.GetIndex();
        auto lastIndex = _models.size() - 1;

        // Notify deletion to model
        auto modelMatch = _storage.modelManager->LookupId(_models[index]);
        _storage.modelManager->UnregisterInstance(modelMatch, _entities[index]);

        super::DestroyComponent(match);

        // Move last to deleted one if it is not the last
        if (index < lastIndex)
        {
            _models[index]     = _models[lastIndex];
            _transforms[index] = _transforms[lastIndex];
        }

        _models.pop_back();
        _transforms.pop_back();
    }

    [[nodiscard]] model_id_t RenderNodeManager::GetModel(const core::Match &match) const
    {
        return _models[match.GetIndex()];
    }

    [[nodiscard]] glm::mat4 RenderNodeManager::GetTransform(const core::Match &match) const
    {
        return _transforms[match.GetIndex()];
    }

} // namespace railguard::rendering
