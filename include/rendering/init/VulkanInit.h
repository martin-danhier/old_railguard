#pragma once

#include "VkInitIncludes.h"
#include "VulkanInitInfo.h"
#include "SwapchainInitInfo.h"

namespace railguard::rendering::init
{
    class VulkanInit
    {
    public:
        /**
         * @brief Inits the code handles of vulkan (instance, surface, device, queue, allocator)
         *
         * @param initInfo A struct containing destination pointers for the various handles.
         */
        static void InitVulkan(const VulkanInitInfo &initInfo);
        static void InitWindowSwapchain(const SwapchainInitInfo &initInfo);
    };
}