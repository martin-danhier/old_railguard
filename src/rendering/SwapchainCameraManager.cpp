#include "../../include/rendering/SwapchainCameraManager.h"

namespace railguard::rendering
{

    SwapchainCameraManager::SwapchainCameraManager(const core::component_id_t defaultComponentCapacity) : ComponentManager(defaultComponentCapacity)
    {
        _enabledCameras.reserve(defaultComponentCapacity);
        _swapchainIds.reserve(defaultComponentCapacity);
    }

    core::Match SwapchainCameraManager::CreateComponent(const core::Entity &entity, const init::SwapchainCameraInitInfo &initInfo)
    {
        // Create a new camera
        _enabledCameras.push_back(initInfo.enabled);
        _swapchainIds.push_back(initInfo.swapchainId);

        // Run boilerplate for entity management
        return this->RegisterComponent(entity);
    }

    void SwapchainCameraManager::DestroyComponent(core::component_id_t index)
    {
        // Run boilerplate deletion
        core::ComponentManager::DestroyComponent(index);

        // Move the last item of vectors to the destroyed index if it is not the last
        core::component_id_t lastIndex = _enabledCameras.size() - 1;
        if (index < lastIndex)
        {
            _enabledCameras[index] = _enabledCameras[lastIndex];
            _swapchainIds[index] = _swapchainIds[lastIndex];
        }
        // Remove the last element
        _enabledCameras.pop_back();
        _swapchainIds.pop_back();
    }

    bool SwapchainCameraManager::GetEnabled(const core::Match &match) const
    {
        return _enabledCameras[match.GetIndex()];
    }

    swapchain_id_t SwapchainCameraManager::GetSwapchainId(const core::Match &match) const
    {
        return _swapchainIds[match.GetIndex()];
    }

    void SwapchainCameraManager::Draw() {
        // For each enabled camera
        for (size_t i = 0; i < _enabledCameras.size(); i++) {
            if (_enabledCameras[i] == true) {

                // Render the appropriate image
                


            }
        }
    }
}