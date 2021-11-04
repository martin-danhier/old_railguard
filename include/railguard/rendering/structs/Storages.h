#pragma once

#define VK_DEFINE_HANDLE(object) typedef struct object##_T* object;
VK_DEFINE_HANDLE(VkDevice)
VK_DEFINE_HANDLE(VkPhysicalDevice)

namespace railguard::rendering::structs
{
    /**
     * @brief Common Storage used with StandaloneManagers to store the vulkan device.
     */
    struct DeviceStorage
    {
      public:
        VkDevice vulkanDevice = nullptr;
    };

    /**
     * @brief Common Storage used with StandaloneManagers to store the vulkan device and the physical device.
     */
    struct FullDeviceStorage
    {
      public:
        VkDevice vulkanDevice                 = nullptr;
        VkPhysicalDevice vulkanPhysicalDevice = nullptr;
    };

} // namespace railguard::rendering::structs
