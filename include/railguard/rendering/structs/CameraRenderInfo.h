#pragma once

#include "railguard/includes/Vulkan.h"

#include <glm/detail/type_quat.hpp>

namespace railguard::rendering::structs
{
    struct CameraRenderInfo
    {
        vk::RenderPassBeginInfo renderPassBeginInfo;
        glm::vec3 position;
        glm::quat rotation;
        glm::mat4 projectionMatrix;
    };
} // namespace railguard::rendering::structs
