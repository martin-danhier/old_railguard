#pragma once

#include "../SwapchainManager.h"
#include "../enums/CameraMode.h"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include "../../utils/Colors.h"

namespace railguard::rendering::init
{
    struct SwapchainCameraInitInfo
    {
    public:
        /**
         * @brief Is the camera enabled or not ? Disabled cameras do not render anything.
         */
        bool enabled = true;
        /**
         * @brief Id of the swapchain used for the camera in the swapchain manager.
         */
        railguard::rendering::swapchain_id_t swapchainId;
        /**
         * @brief CameraMode of the camera. Defines whether it will use a orthographic or perspective projection matrix.
         *
         * By default, the camera mode will be CameraMode::Perspective.
         */
        enums::CameraMode mode = enums::CameraMode::Perspective;
        /**
         * @brief Position of the camera in the world. Defined in X, Y and Z coordinates.
         * @see Coordinates documentation (TODO)
         */
        glm::vec3 position = glm::vec3(0.f, 0.f, 0.f);
        /**
         * @brief Rotation of the camera in the world. Defined using a quarternion.
         * @see Rotations documentation (TODO)
         */
        glm::quat rotation = glm::quat(0.f, 0.f, 0.f, 1.f);
        /**
         * @brief Field of view of the camera, in degrees. Ignored in orthographic mode.
         */
        float fieldOfView = 70.f;
        /**
         * @brief First param of the camera mode.
         *
         * In CameraMode::Orthographic mode, it represents the horizontal distance between the center and the border.
         *
         * In CameraMode::Perspective mode, it represents the distance of the near plane.
         */
        float modeParam1 = 0.1f;
        /**
         * @brief Second param of the camera mode.
         *
         * In CameraMode::Orthographic mode, it represents the vertical distance between the center and the border.
         *
         * In CameraMode::Perspective mode, it represents the distance of the far plane.
         */
        float modeParam2 = 100.f;
        /**
         * @brief Color used to clear the image.
         */
        utils::Color clearColor = utils::GetColor(1.0f, 1.0f, 1.0f, 1.0f);
    };
}