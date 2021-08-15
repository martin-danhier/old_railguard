#pragma once

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
        static void InitVulkan(const struct VulkanInitInfo &initInfo);
        static void InitWindowSwapchain(const struct SwapchainInitInfo &initInfo);
        
    };
}