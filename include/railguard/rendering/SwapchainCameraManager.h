#pragma once

#include <glm/glm.hpp>
#include <railguard/core/ComponentManager.h>
#include <railguard/rendering/Ids.h>
#include <vector>

#define VK_DEFINE_HANDLE(object) typedef struct object##_T* object;
VK_DEFINE_HANDLE(VkClearValue);
VK_DEFINE_HANDLE(VkRenderPass);

namespace railguard::rendering
{
    // Forward declarations
    namespace init
    {
        struct SwapchainCameraInitInfo;
    }
    namespace structs
    {
        struct CameraRenderInfo;
    }
    namespace enums
    {
        enum class CameraMode : uint32_t;
    }

    // Define storage type
    struct SwapchainCameraManagerStorage
    {
        class SwapchainManager *swapchainManager;
    };

    /**
     * @brief A swapchain camera is a camera that renders to a swapchain.
     *
     * This component can be attached to entities in order to give them a camera.
     *
     * These cameras can be used to render images to the window, for example. To render to a texture, see RenderTextureCameraManager
     * (TODO).
     */
    class SwapchainCameraManager : public core::ComponentManager<SwapchainCameraManagerStorage>
    {
      private:
        // Typedef the parent type to make it easier to call from the methods
        typedef core::ComponentManager<SwapchainCameraManagerStorage> super;

        /**
         * @brief Is the camera enabled or not ? Disabled cameras do not render anything.
         */
        std::vector<bool> _enabledCameras;
        /**
         * @brief CameraMode of the camera. Defines whether it will use a orthographic or perspective projection matrix.
         *
         * By default, the camera mode will be CameraMode::Perspective.
         */
        std::vector<enums::CameraMode> _cameraModes;
        /**
         * @brief Position of the camera in the world. Defined in X, Y and Z coordinates.
         * @see Coordinates documentation (TODO)
         */
        std::vector<glm::vec3> _cameraPositions;
        /**
         * @brief Rotation of the camera in the world. Defined using a quarternion.
         * @see Rotations documentation (TODO)
         */
        std::vector<glm::quat> _cameraRotations;
        /**
         * @brief Field of view of the camera, in degrees. Ignored in orthographic mode.
         */
        std::vector<float> _fieldsOfView;
        /**
         * @brief First param of the camera mode.
         *
         * In CameraMode::Orthographic mode, it represents the horizontal distance between the center and the border.
         *
         * In CameraMode::Perspective mode, it represents the distance of the near plane.
         */
        std::vector<float> _modeParams1;
        /**
         * @brief Second param of the camera mode.
         *
         * In CameraMode::Orthographic mode, it represents the vertical distance between the center and the border.
         *
         * In CameraMode::Perspective mode, it represents the distance of the far plane.
         */
        std::vector<float> _modeParams2;
        /**
         * @brief Id of the swapchain used for the camera in the swapchain manager.
         */
        std::vector<swapchain_id_t> _swapchainIds;
        /**
         * @brief Aspect ratio of the camera. Only used in perspective mode.
         */
        std::vector<float> _aspectRatios;
        /**
         * @brief Color used to clear the image.
         */
        std::vector<VkClearValue> _clearColors;

        uint32_t _lastNbOfActiveCameras = 1;

      public:
        SwapchainCameraManager(SwapchainCameraManagerStorage storage, size_t defaultComponentCapacity);

        void Clear() override;

        /**
         * @brief Create a new Camera component for the given entity
         *
         * @param entity Entity for which a component will be created
         * @param initInfo Info of the created camera
         * @return core::Match linking the entity to its component slot
         */
        core::Match CreateComponent(const core::Entity &entity, const init::SwapchainCameraInitInfo &initInfo);
        void DestroyComponent(const core::Match &match) override;

        // Getters

        [[nodiscard]] bool GetEnabled(const core::Match &match) const;
        [[nodiscard]] swapchain_id_t GetSwapchainId(const core::Match &match) const;
        [[nodiscard]] enums::CameraMode GetCameraMode(const core::Match &match) const;
        [[nodiscard]] glm::vec3 GetPosition(const core::Match &match) const;
        [[nodiscard]] glm::quat GetRotation(const core::Match &match) const;
        [[nodiscard]] float GetFieldOfView(const core::Match &match) const;
        [[nodiscard]] float GetModeParam1(const core::Match &match) const;
        [[nodiscard]] float GetModeParam2(const core::Match &match) const;
        [[nodiscard]] float GetAspectRatio(const core::Match &match) const;
        [[nodiscard]] VkClearValue GetClearColor(const core::Match &match) const;

        // Others
        [[nodiscard]] glm::mat4 GetProjectionMatrix(const core::Match &match) const;
        [[nodiscard]] glm::mat4 GetProjectionMatrix(size_t index) const;

        // Systems

        /**
         * @brief For each active camera, generate render infos.
         *
         * @return A vector containing, for each active camera, a struct containing the data
         * that the Renderer needs to render to this camera.
         */
        [[nodiscard]] std::vector<structs::CameraRenderInfo> GetRenderInfos(const VkRenderPass &renderPass);
    };
} // namespace railguard::rendering