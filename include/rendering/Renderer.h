#pragma once
#include "core/ComponentManager.h"

namespace railguard::rendering
{

    class Renderer : public core::ComponentManager
    {
    private:
        
    public:
        explicit Renderer(const size_t defaultComponentCapacity);
        core::ComponentMatch CreateComponent(const core::Entity &entity, int test);
        void DestroyComponent(size_t index) override;
    };

} // namespace railguard::rendering
