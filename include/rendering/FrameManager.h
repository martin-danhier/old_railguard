#pragma once

#include "../includes/Vulkan.h"
#include "./Settings.h"

namespace railguard::rendering
{
    struct FrameData {
        vk::CommandPool commandPool;
        vk::CommandBuffer commandBuffer;
        vk::Fence renderFence;
        vk::Semaphore presentSemaphore;
        vk::Semaphore renderSemaphore;
    };

    class FrameManager
    {
    private:
        // Ref to device
        vk::Device _device = nullptr;

        // Data itself
        // We use arrays instead of vectors because the size is constant
        // We also don't need to handle ids like in other managers
        vk::CommandPool _commandPools[NB_OVERLAPPING_FRAMES];
        vk::CommandBuffer _commandBuffers[NB_OVERLAPPING_FRAMES];
        vk::Semaphore _presentSemaphores[NB_OVERLAPPING_FRAMES];
        vk::Semaphore _renderSemaphores[NB_OVERLAPPING_FRAMES];
        vk::Fence _renderFences[NB_OVERLAPPING_FRAMES];

#ifdef USE_ADVANCED_CHECKS
        // In debug mode, keep track of the init status to ensure that Init is called first
        bool _initialized = false;
#endif

    public:
        void Init(const vk::Device &device, uint32_t graphicsQueueFamily);
        void Cleanup();
        ~FrameManager();

        [[nodiscard]] const FrameData GetFrame(uint32_t index) const;
        [[nodiscard]] const vk::CommandPool GetCommandPool(uint32_t index) const;
        [[nodiscard]] const vk::CommandBuffer GetCommandBuffer(uint32_t index) const;
        [[nodiscard]] const vk::Fence GetRenderFence(uint32_t index) const;
        [[nodiscard]] const vk::Semaphore GetRenderSemaphore(uint32_t index) const;
        [[nodiscard]] const vk::Semaphore GetPresentSemaphore(uint32_t index) const;
    };
}