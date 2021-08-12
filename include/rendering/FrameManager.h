#pragma once

#include "../includes/Vulkan.h"
#include "Ids.h"
#include "./Settings.h"
#include "AllocationManager.h"

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

        // Counter of frames since the start of the program
        uint64_t _currentFrameNumber = 1;

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
        /**
         * @brief Ends the current frame and increments the frame number.
         *
         * @return uint64_t The frame number of the next frame.
         */
        uint64_t FinishFrame();

        void WaitForFence(const vk::Fence &fence) const;
        void WaitForCurrentFence() const;
        void WaitForAllFences() const;

        vk::CommandBuffer BeginRecording();
        void EndRecordingAndSubmit(const vk::Queue &graphicsQueue);

        [[nodiscard]] uint64_t GetFrameNumber() const;
        [[nodiscard]] uint32_t GetCurrentFrameIndex() const;

        [[nodiscard]] const FrameData GetCurrentFrame() const;
        [[nodiscard]] const vk::CommandPool GetCurrentCommandPool() const;
        [[nodiscard]] const vk::CommandBuffer GetCurrentCommandBuffer() const;
        [[nodiscard]] const vk::Fence GetCurrentRenderFence() const;
        [[nodiscard]] const vk::Semaphore* GetCurrentRenderSemaphore() const;
        [[nodiscard]] const vk::Semaphore* GetCurrentPresentSemaphore() const;

        [[nodiscard]] const FrameData GetFrame(uint32_t index) const;
        [[nodiscard]] const vk::CommandPool GetCommandPool(uint32_t index) const;
        [[nodiscard]] const vk::CommandBuffer GetCommandBuffer(uint32_t index) const;
        [[nodiscard]] const vk::Fence GetRenderFence(uint32_t index) const;
        [[nodiscard]] const vk::Semaphore* GetRenderSemaphore(uint32_t index) const;
        [[nodiscard]] const vk::Semaphore* GetPresentSemaphore(uint32_t index) const;
    };
}