#pragma once

#include "../../../include/includes/Vulkan.h"

namespace railguard::rendering::structs
{
    /**
     * @brief Common Storage used with StandaloneManagers to store the vulkan device.
     */
    struct DeviceStorage
    {
    public:
        vk::Device vulkanDevice = nullptr;
    };

    /**
     * @brief Common Storage used with StandaloneManagers to store the vulkan device and the physical device.
     */
    struct FullDeviceStorage
    {
    public:
        vk::Device vulkanDevice = nullptr;
        vk::PhysicalDevice vulkanPhysicalDevice = nullptr;
    };

} // namespace railguard::rendering::structs
