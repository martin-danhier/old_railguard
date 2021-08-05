#pragma once

#include "../core/ComponentManager.h"
#include "Ids.h"
#include "ModelManager.h"
#include <glm/glm.hpp>

namespace railguard::rendering
{

    struct RenderNodeManagerStorage
    {
        ModelManager *modelManager;
    };

    class RenderNodeManager : public core::ComponentManager<RenderNodeManagerStorage>
    {
    private:
        typedef core::ComponentManager<RenderNodeManagerStorage> super;

        std::vector<model_id_t> _models;

        // Transform
        std::vector<glm::mat4> _transforms;

    public:
        void Init(RenderNodeManagerStorage storage, const size_t defaultComponentCapacity = 50);
        void Clear();
        core::Match CreateComponent(const core::Entity &entity, model_id_t model);
        void DestroyComponent(const core::Match &match);

        // Getters
        [[nodiscard]] model_id_t GetModel(const core::Match &match) const;
        [[nodiscard]] glm::mat4 GetTransform(const core::Match &match) const;

    };

} // namespace railguard::rendering
