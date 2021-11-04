#pragma once

#include <cstdint>
#include <railguard/rendering/Settings.h>

using std::uint32_t;
using std::uint64_t;
#define VK_DEFINE_HANDLE(object) typedef struct object##_T *(object);
VK_DEFINE_HANDLE(VkCommandPool)
VK_DEFINE_HANDLE(VkCommandBuffer)
VK_DEFINE_HANDLE(VkFence)
VK_DEFINE_HANDLE(VkSemaphore)
VK_DEFINE_HANDLE(VkDevice)
VK_DEFINE_HANDLE(VkQueue)

namespace railguard::rendering
{
    struct FrameData
    {
        VkCommandPool commandPool;
        VkCommandBuffer commandBuffer;
        VkFence renderFence;
        VkSemaphore presentSemaphore;
        VkSemaphore renderSemaphore;
    };

    class FrameManager
    {
      private:
        // Ref to device
        VkDevice _device = nullptr;

        // Counter of frames since the start of the program
        uint64_t _currentFrameNumber = 1;

        // Data itself
        // We use arrays instead of vectors because the size is constant
        // We also don't need to handle ids like in other managers
        VkCommandPool _commandPools[NB_OVERLAPPING_FRAMES];
        VkCommandBuffer _commandBuffers[NB_OVERLAPPING_FRAMES];
        VkSemaphore _presentSemaphores[NB_OVERLAPPING_FRAMES];
        VkSemaphore _renderSemaphores[NB_OVERLAPPING_FRAMES];
        VkFence _renderFences[NB_OVERLAPPING_FRAMES];

      public:
        FrameManager(const VkDevice &device, uint32_t graphicsQueueFamily);
        ~FrameManager();
        /**
         * @brief Ends the current frame and increments the frame number.
         *
         * @return uint64_t The frame number of the next frame.
         */
        uint64_t FinishFrame();

        void WaitForFence(const VkFence &fence) const;
        void WaitForCurrentFence() const;
        void WaitForAllFences() const;

        [[nodiscard]] VkCommandBuffer BeginRecording() const;
        void EndRecordingAndSubmit(const VkQueue &graphicsQueue) const;

        [[nodiscard]] uint64_t GetFrameNumber() const;
        [[nodiscard]] uint32_t GetCurrentFrameIndex() const;

        [[nodiscard]] FrameData GetCurrentFrame() const;
        [[nodiscard]] VkCommandPool GetCurrentCommandPool() const;
        [[nodiscard]] VkCommandBuffer GetCurrentCommandBuffer() const;
        [[nodiscard]] VkFence GetCurrentRenderFence() const;
        [[nodiscard]] const VkSemaphore *GetCurrentRenderSemaphore() const;
        [[nodiscard]] const VkSemaphore *GetCurrentPresentSemaphore() const;

        [[nodiscard]] FrameData GetFrame(uint32_t index) const;
        [[nodiscard]] VkCommandPool GetCommandPool(uint32_t index) const;
        [[nodiscard]] VkCommandBuffer GetCommandBuffer(uint32_t index) const;
        [[nodiscard]] VkFence GetRenderFence(uint32_t index) const;
        [[nodiscard]] const VkSemaphore *GetRenderSemaphore(uint32_t index) const;
        [[nodiscard]] const VkSemaphore *GetPresentSemaphore(uint32_t index) const;
    };
} // namespace railguard::rendering