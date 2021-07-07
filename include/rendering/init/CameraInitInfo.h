#pragma once

#include "../SwapchainManager.h"

namespace railguard::rendering::init
{
    struct SwapchainCameraInitInfo
    {
    public:
        bool enabled;
        railguard::rendering::swapchain_id_t swapchainId;
    };
}