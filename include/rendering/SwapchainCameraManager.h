#pragma once

#include <vector>
#include "../core/ComponentManager.h"

namespace railguard::rendering
{
    class SwapchainCameraManager : core::ComponentManager
    {
    private:
        std::vector<bool> enabled;


    public:
        explicit SwapchainCameraManager(const size_t defaultComponentCapacity);
        ~SwapchainCameraManager();
        core::Match CreateComponent(const core::Entity &entity);
        void DestroyComponent(size_t index) override;
    };
}