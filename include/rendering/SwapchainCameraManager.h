#pragma once

#include <vector>
#include "./init/CameraInitInfo.h"
#include "../core/ComponentManager.h"
#include "./SwapchainManager.h"

namespace railguard::rendering
{
    class SwapchainCameraManager : core::ComponentManager
    {
    private:
        /**
         * @brief Is the camera enabled or not ? Disabled cameras do not render anything.
         */
        std::vector<bool> _enabledCameras;
        /**
         * @brief Id of the swapchain used for the camera in the swapchain manager.
         */
        std::vector<swapchain_id_t> _swapchainIds;

    public:
        explicit SwapchainCameraManager(const core::component_id_t defaultComponentCapacity = 1);

        /**
         * @brief Create a new Camera component for the given entity
         *
         * @param entity Entity for which a component will be created
         * @param initInfo Info of the created camera
         * @return core::Match linking the entity to its component slot
         */
        core::Match CreateComponent(const core::Entity &entity, const init::SwapchainCameraInitInfo &initInfo);
        void DestroyComponent(core::component_id_t index) override;

        // Getters

        [[nodiscard]] bool GetEnabled(const core::Match &match) const;
        [[nodiscard]] swapchain_id_t GetSwapchainId(const core::Match &match) const;

        // Systems

        /**
         * @brief For each camera, render an image.
         */
        void Draw();
    };
}