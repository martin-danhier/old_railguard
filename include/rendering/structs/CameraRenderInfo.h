#pragma once

#include "../../includes/Vulkan.h"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace railguard::rendering::structs
{
    struct CameraRenderInfo {
        vk::RenderPassBeginInfo renderPassBeginInfo;
        glm::vec3 position;
        glm::quat rotation;
        glm::mat4 projectionMatrix;
    };
} // namespace railguard::rendering::structs
