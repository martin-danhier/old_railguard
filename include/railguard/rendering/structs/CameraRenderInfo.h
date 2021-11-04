#pragma once

#include <glm/gtc/quaternion.hpp>
#include <vulkan/vulkan.h>

namespace railguard::rendering::structs
{
    struct CameraRenderInfo
    {
        VkRenderPassBeginInfo renderPassBeginInfo;
        glm::vec3 position;
        glm::quat rotation;
        glm::mat4 projectionMatrix;
    };
} // namespace railguard::rendering::structs
