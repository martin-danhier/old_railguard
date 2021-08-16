#include "railguard/rendering/SwapchainCameraManager.h"

#include "railguard/includes/Vulkan.h"
#include "railguard/rendering/init/CameraInitInfo.h"
#include "railguard/rendering/structs/CameraRenderInfo.h"
#include "railguard/utils/AdvancedCheck.h"

namespace railguard::rendering
{
    SwapchainCameraManager::SwapchainCameraManager(SwapchainCameraManagerStorage storage,
                                                   const core::component_idx_t defaultComponentCapacity)
        : super(storage, defaultComponentCapacity)
    {
        // Init vectors that were added by this component
        _enabledCameras.reserve(defaultComponentCapacity);
        _swapchainIds.reserve(defaultComponentCapacity);
        _cameraModes.reserve(defaultComponentCapacity);
        _cameraPositions.reserve(defaultComponentCapacity);
        _cameraRotations.reserve(defaultComponentCapacity);
        _fieldsOfView.reserve(defaultComponentCapacity);
        _modeParams1.reserve(defaultComponentCapacity);
        _modeParams2.reserve(defaultComponentCapacity);
        _aspectRatios.reserve(defaultComponentCapacity);
        _clearColors.reserve(defaultComponentCapacity);
    }

    core::Match SwapchainCameraManager::CreateComponent(const core::Entity &entity, const init::SwapchainCameraInitInfo &initInfo)
    {
        // Create a new camera
        _enabledCameras.push_back(initInfo.enabled);
        _swapchainIds.push_back(initInfo.swapchainId);
        _cameraModes.push_back(initInfo.mode);
        _cameraPositions.push_back(initInfo.position);
        _cameraRotations.push_back(initInfo.rotation);
        _fieldsOfView.push_back(initInfo.fieldOfView);
        _modeParams1.push_back(initInfo.modeParam1);
        _modeParams2.push_back(initInfo.modeParam2);
        _clearColors.emplace_back(initInfo.clearColor);

        // Get aspect ratio if it is in perspective mode
        if (initInfo.mode == enums::CameraMode::Perspective)
        {
            // Get swapchain
            auto swapchain = _storage.swapchainManager->LookupId(initInfo.swapchainId);
            // Get extent
            auto extent = _storage.swapchainManager->GetViewportExtent(swapchain);

            // Save aspect ratio
            _aspectRatios.push_back(static_cast<float>(extent.width) / static_cast<float>(extent.height));
        }
        // In orthographic mode, we don't really care
        else
        {
            _aspectRatios.push_back(0.0f);
        }

        // Run boilerplate for entity management
        return super::RegisterComponent(entity);
    }

    void SwapchainCameraManager::DestroyComponent(const core::Match &match)
    {
        // Get index
        auto index             = match.GetIndex();
        const size_t lastIndex = _entities.size() - 1;

        // Run boilerplate deletion
        super::DestroyComponent(match);

        if (index < lastIndex)
        {
            _enabledCameras[index]  = _enabledCameras[lastIndex];
            _swapchainIds[index]    = _swapchainIds[lastIndex];
            _cameraModes[index]     = _cameraModes[lastIndex];
            _cameraPositions[index] = _cameraPositions[lastIndex];
            _cameraRotations[index] = _cameraRotations[lastIndex];
            _fieldsOfView[index]    = _fieldsOfView[lastIndex];
            _modeParams1[index]     = _modeParams1[lastIndex];
            _modeParams2[index]     = _modeParams2[lastIndex];
            _aspectRatios[index]    = _aspectRatios[lastIndex];
            _clearColors[index]     = _clearColors[lastIndex];
        }
        // Remove the last element
        _enabledCameras.pop_back();
        _swapchainIds.pop_back();
        _cameraModes.pop_back();
        _cameraPositions.pop_back();
        _cameraRotations.pop_back();
        _fieldsOfView.pop_back();
        _modeParams1.pop_back();
        _modeParams2.pop_back();
        _aspectRatios.pop_back();
        _clearColors.pop_back();
    }

    void SwapchainCameraManager::Clear()
    {
        super::Clear();

        // Clear every vector
        _enabledCameras.clear();
        _swapchainIds.clear();
        _cameraModes.clear();
        _cameraPositions.clear();
        _cameraRotations.clear();
        _fieldsOfView.clear();
        _modeParams1.clear();
        _modeParams2.clear();
        _aspectRatios.clear();
        _clearColors.clear();
    }

    bool SwapchainCameraManager::GetEnabled(const core::Match &match) const
    {
        return _enabledCameras[match.GetIndex()];
    }

    swapchain_id_t SwapchainCameraManager::GetSwapchainId(const core::Match &match) const
    {
        return _swapchainIds[match.GetIndex()];
    }

    enums::CameraMode SwapchainCameraManager::GetCameraMode(const core::Match &match) const
    {
        return _cameraModes[match.GetIndex()];
    }

    glm::vec3 SwapchainCameraManager::GetPosition(const core::Match &match) const
    {
        return _cameraPositions[match.GetIndex()];
    }

    glm::quat SwapchainCameraManager::GetRotation(const core::Match &match) const
    {
        return _cameraRotations[match.GetIndex()];
    }

    float SwapchainCameraManager::GetFieldOfView(const core::Match &match) const
    {
        return _fieldsOfView[match.GetIndex()];
    }

    float SwapchainCameraManager::GetModeParam1(const core::Match &match) const
    {
        return _modeParams1[match.GetIndex()];
    }

    float SwapchainCameraManager::GetModeParam2(const core::Match &match) const
    {
        return _modeParams2[match.GetIndex()];
    }
    float SwapchainCameraManager::GetAspectRatio(const core::Match &match) const
    {
        return _aspectRatios[match.GetIndex()];
    }
    vk::ClearValue SwapchainCameraManager::GetClearColor(const core::Match &match) const
    {
        return _clearColors[match.GetIndex()];
    }

    glm::mat4 SwapchainCameraManager::GetProjectionMatrix(const core::Match &match) const
    {
        auto index = match.GetIndex();
        return GetProjectionMatrix(index);
    }
    glm::mat4 SwapchainCameraManager::GetProjectionMatrix(size_t index) const
    {
        // Perspective
        if (_cameraModes[index] == enums::CameraMode::Perspective)
        {
            return glm::perspective(
                glm::radians(_fieldsOfView[index]), _aspectRatios[index], _modeParams1[index], _modeParams2[index]);
        }
        // Orthographic
        else
        {
            // TODO check if correct
            return glm::ortho(-_modeParams1[index], _modeParams1[index], _modeParams2[index], -_modeParams2[index]);
        }
    }

    std::vector<structs::CameraRenderInfo> SwapchainCameraManager::GetRenderInfos(const vk::RenderPass &renderPass)
    {
        std::vector<structs::CameraRenderInfo> renderInfos;
        // Reserve the vector with as much slots as the last frames.
        // Most of the time, the number of active cameras wont change, and the estimation will be
        // on point.
        // Sometimes, the number will change, in this case the estimation would be slightly off, but not by a lot.
        renderInfos.reserve(_lastNbOfActiveCameras);

        // Variables to store the value that we get from the swapchain
        // Multiple cameras could reference the same swapchain
        // If they follow each other, these values will not be fetched again
        core::Match swapchainMatch;
        swapchain_id_t lastSwapchainId   = 0;
        uint32_t lastSwapchainImageIndex = 0;
        vk::Extent2D lastSwapchainExtent;

        // For each enabled camera
        for (size_t i = 0; i < _enabledCameras.size(); i++)
        {
            if (_enabledCameras[i])
            {
                // The swapchain of this camera is not the same as the previous one
                if (lastSwapchainId != _swapchainIds[i])
                {
                    // Get the swapchain
                    swapchainMatch          = _storage.swapchainManager->LookupId(_swapchainIds[i]);
                    lastSwapchainExtent     = _storage.swapchainManager->GetViewportExtent(swapchainMatch);
                    lastSwapchainImageIndex = _storage.swapchainManager->RequestNextImageIndex(swapchainMatch);
                    lastSwapchainId         = _swapchainIds[i];
                }

                // Generate a render info struct
                structs::CameraRenderInfo info {
                    .renderPassBeginInfo =
                        {
                            .renderPass  = renderPass,
                            .framebuffer = _storage.swapchainManager->GetFramebuffers(swapchainMatch)[lastSwapchainImageIndex],
                            .renderArea =
                                {
                                    .offset = {0, 0},
                                    .extent = lastSwapchainExtent,
                                },
                            .clearValueCount = 1,
                            .pClearValues    = &_clearColors[i],
                        },
                    .position         = _cameraPositions[i],
                    .rotation         = _cameraRotations[i],
                    .projectionMatrix = GetProjectionMatrix(i),
                };
                renderInfos.push_back(info);
            }
        }

        // Update estimation for next frame
        if (_lastNbOfActiveCameras != renderInfos.size())
        {
            _lastNbOfActiveCameras = static_cast<uint32_t>(renderInfos.size());
        }

        ADVANCED_CHECK(_lastNbOfActiveCameras > 0, "There must be at least one active camera !");

        return renderInfos;
    }
} // namespace railguard::rendering