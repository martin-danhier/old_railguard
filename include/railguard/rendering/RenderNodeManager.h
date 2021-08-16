#pragma once

#include <glm/glm.hpp>
#include <railguard/core/ComponentManager.h>
#include <railguard/rendering/Ids.h>

namespace railguard::rendering
{
    struct RenderNodeManagerStorage
    {
        class ModelManager *modelManager;
    };

    class RenderNodeManager : public core::ComponentManager<RenderNodeManagerStorage>
    {
      private:
        typedef core::ComponentManager<RenderNodeManagerStorage> super;

        std::vector<model_id_t> _models;

        // Transform
        std::vector<glm::mat4> _transforms;

      public:
        explicit RenderNodeManager(RenderNodeManagerStorage storage, size_t defaultComponentCapacity = 50);
        void Clear() override;
        core::Match CreateComponent(const core::Entity &entity, model_id_t model);
        void DestroyComponent(const core::Match &match) override;

        // Getters
        [[nodiscard]] model_id_t GetModel(const core::Match &match) const;
        [[nodiscard]] glm::mat4 GetTransform(const core::Match &match) const;
    };

} // namespace railguard::rendering
