#pragma once

#include "railguard/includes/Vulkan.h"
#include "railguard/rendering/Settings.h"

namespace railguard::rendering
{
    struct FrameData
    {
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

      public:
        FrameManager(const vk::Device &device, uint32_t graphicsQueueFamily);
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

        [[nodiscard]] vk::CommandBuffer BeginRecording() const;
        void EndRecordingAndSubmit(const vk::Queue &graphicsQueue) const;

        [[nodiscard]] uint64_t GetFrameNumber() const;
        [[nodiscard]] uint32_t GetCurrentFrameIndex() const;

        [[nodiscard]] FrameData GetCurrentFrame() const;
        [[nodiscard]] vk::CommandPool GetCurrentCommandPool() const;
        [[nodiscard]] vk::CommandBuffer GetCurrentCommandBuffer() const;
        [[nodiscard]] vk::Fence GetCurrentRenderFence() const;
        [[nodiscard]] const vk::Semaphore *GetCurrentRenderSemaphore() const;
        [[nodiscard]] const vk::Semaphore *GetCurrentPresentSemaphore() const;

        [[nodiscard]] FrameData GetFrame(uint32_t index) const;
        [[nodiscard]] vk::CommandPool GetCommandPool(uint32_t index) const;
        [[nodiscard]] vk::CommandBuffer GetCommandBuffer(uint32_t index) const;
        [[nodiscard]] vk::Fence GetRenderFence(uint32_t index) const;
        [[nodiscard]] const vk::Semaphore *GetRenderSemaphore(uint32_t index) const;
        [[nodiscard]] const vk::Semaphore *GetPresentSemaphore(uint32_t index) const;
    };
} // namespace railguard::rendering