#include "../../include/rendering/FrameManager.h"
#include "../../include/utils/AdvancedCheck.h"

#ifdef USE_ADVANCED_CHECKS
#include <iostream>

// Define local error messages
#define INITIALIZED_TWICE_ERROR "FrameManager should not be initialized twice."
#define NOT_INITIALIZED_ERROR "FrameManager should be initialized with Init before calling this method."
#define NOT_CLEANED_ERROR "FrameManager should be cleaned up with Cleanup before it is destroyed."
#define INDEX_OUT_OF_RANGE_ERROR "The provided index references an nonexisting frame. The index must be lower than NB_OVERLAPPING_FRAMES."
#endif

namespace railguard::rendering
{
    void FrameManager::Init(const vk::Device &device, uint32_t graphicsQueueFamily)
    {
        ADVANCED_CHECK(!_initialized, INITIALIZED_TWICE_ERROR);

        _device = vk::Device(device);

        // Prepare create info structs
        vk::CommandPoolCreateInfo commandPoolCreateInfo{
            .flags = vk::CommandPoolCreateFlagBits::eResetCommandBuffer,
            .queueFamilyIndex = graphicsQueueFamily,
        };

        // For each frame
        for (uint32_t i = 0; i < NB_OVERLAPPING_FRAMES; i++)
        {
            // Init it

            // Create command pool
            _commandPools[i] = device.createCommandPool(commandPoolCreateInfo);

            // Create command buffer
            vk::CommandBufferAllocateInfo commandBufferAllocateInfo{
                .commandPool = _commandPools[i],
                .level = vk::CommandBufferLevel::ePrimary,
                .commandBufferCount = 1,
            };
            _commandBuffers[i] = device.allocateCommandBuffers(commandBufferAllocateInfo)[0];
        }

        // Create fences
        // We separate the for loops so that we can really focus on one vector at a time

        vk::FenceCreateInfo fenceCreateInfo{
            .flags = vk::FenceCreateFlagBits::eSignaled,
        };

        for (auto &fence : _renderFences)
        {
            fence = device.createFence(fenceCreateInfo);
        }

        // Create semaphores

        vk::SemaphoreCreateInfo semaphoreCreateInfo{};

        for (auto &presentSemaphore : _presentSemaphores)
        {
            presentSemaphore = device.createSemaphore(semaphoreCreateInfo);
        }
        for (auto &renderSemaphore : _renderSemaphores)
        {
            renderSemaphore = device.createSemaphore(semaphoreCreateInfo);
        }

#ifdef USE_ADVANCED_CHECKS
        _initialized = true;
#endif
    }

    void FrameManager::Cleanup()
    {
        ADVANCED_CHECK(_initialized, NOT_INITIALIZED_ERROR);

        // Destroy semaphores

        for (auto &renderSemaphore : _renderSemaphores)
        {
            _device.destroySemaphore(renderSemaphore);
            renderSemaphore = nullptr;
        }
        for (auto &presentSemaphore : _presentSemaphores)
        {
            _device.destroySemaphore(presentSemaphore);
            presentSemaphore = nullptr;
        }

        // Destroy fences

        for (auto &fence : _renderFences)
        {
            _device.destroyFence(fence);
            fence = nullptr;
        }

        // Destroy command pools

        for (auto &pool : _commandPools)
        {
            _device.destroyCommandPool(pool);
            pool = nullptr;
        }

#ifdef USE_ADVANCED_CHECKS
        // Back to the beginning
        _initialized = false;
#endif
    }

    FrameManager::~FrameManager()
    {
        ADVANCED_CHECK(!_initialized, NOT_CLEANED_ERROR);
    }

    const FrameData FrameManager::GetFrame(uint32_t index) const
    {
        ADVANCED_CHECK(_initialized, NOT_INITIALIZED_ERROR);
        ADVANCED_CHECK(index < NB_OVERLAPPING_FRAMES, INDEX_OUT_OF_RANGE_ERROR);

        return FrameData{
            .commandPool = _commandPools[index],
            .commandBuffer = _commandBuffers[index],
            .renderFence = _renderFences[index],
            .presentSemaphore = _presentSemaphores[index],
            .renderSemaphore = _renderSemaphores[index],
        };
    }

    void FrameManager::WaitForFence(const vk::Fence &fence) const 
	{
		// Wait for fences
		auto waitResult = _device.waitForFences(fence, true, WAIT_FOR_FENCES_TIMEOUT);
        // Reset it
        _device.resetFences(fence);

		if (waitResult != vk::Result::eSuccess)
		{
			throw std::runtime_error("Error while waiting for fences");
		}
	}

    void FrameManager::WaitForCurrentFence() const {
        WaitForFence(GetCurrentRenderFence());
    }

    void FrameManager::WaitForAllFences() const {
        auto waitResult = _device.waitForFences(NB_OVERLAPPING_FRAMES, _renderFences, true, WAIT_FOR_FENCES_TIMEOUT);
        if (waitResult != vk::Result::eSuccess)
		{
			throw std::runtime_error("Error while waiting for fences");
		}
    }

    vk::CommandBuffer FrameManager::BeginRecording() {
        auto cmd = GetCurrentCommandBuffer();

        // Reset command buffer
        cmd.reset();

        // Begin recording
        vk::CommandBufferBeginInfo cmdBeginInfo{
			.flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit,
		};
        cmd.begin(cmdBeginInfo);

        return cmd;
    }

    void FrameManager::EndRecordingAndSubmit(const vk::Queue &graphicsQueue) {
        // Get handles we will need
        auto frameIndex = GetCurrentFrameIndex();
        auto cmd = GetCommandBuffer(frameIndex);
        auto presentSemaphore = GetPresentSemaphore(frameIndex);
        auto renderSemaphore = GetRenderSemaphore(frameIndex);
        auto renderFence = GetRenderFence(frameIndex);

        // End recording
        cmd.end();

        // Submit to the graphics queue
		vk::PipelineStageFlags waitStage = vk::PipelineStageFlagBits::eColorAttachmentOutput;
		vk::SubmitInfo submitInfo{
			// Wait until the image to render to is ready
			.waitSemaphoreCount = 1,
			.pWaitSemaphores = presentSemaphore,
			// Pipeline stage
			.pWaitDstStageMask = &waitStage,
			// Link the command buffer
			.commandBufferCount = 1,
			.pCommandBuffers = &cmd,
			// Signal the render semaphore
			.signalSemaphoreCount = 1,
			.pSignalSemaphores = renderSemaphore,
		};
		graphicsQueue.submit(submitInfo, renderFence);
    }

    // Getters

    const vk::CommandPool FrameManager::GetCommandPool(uint32_t index) const
    {
        ADVANCED_CHECK(_initialized, NOT_INITIALIZED_ERROR);
        ADVANCED_CHECK(index < NB_OVERLAPPING_FRAMES, INDEX_OUT_OF_RANGE_ERROR);

        return _commandPools[index];
    }
    const vk::CommandBuffer FrameManager::GetCommandBuffer(uint32_t index) const
    {
        ADVANCED_CHECK(_initialized, NOT_INITIALIZED_ERROR);
        ADVANCED_CHECK(index < NB_OVERLAPPING_FRAMES, INDEX_OUT_OF_RANGE_ERROR);

        return _commandBuffers[index];
    }
    const vk::Fence FrameManager::GetRenderFence(uint32_t index) const
    {
        ADVANCED_CHECK(_initialized, NOT_INITIALIZED_ERROR);
        ADVANCED_CHECK(index < NB_OVERLAPPING_FRAMES, INDEX_OUT_OF_RANGE_ERROR);

        return _renderFences[index];
    }
    const vk::Semaphore* FrameManager::GetRenderSemaphore(uint32_t index) const
    {
        ADVANCED_CHECK(_initialized, NOT_INITIALIZED_ERROR);
        ADVANCED_CHECK(index < NB_OVERLAPPING_FRAMES, INDEX_OUT_OF_RANGE_ERROR);

        return &_renderSemaphores[index];
    }
    const vk::Semaphore* FrameManager::GetPresentSemaphore(uint32_t index) const
    {
        ADVANCED_CHECK(_initialized, NOT_INITIALIZED_ERROR);
        ADVANCED_CHECK(index < NB_OVERLAPPING_FRAMES, INDEX_OUT_OF_RANGE_ERROR);

        return &_presentSemaphores[index];
    }

    uint64_t FrameManager::FinishFrame() {
        return _currentFrameNumber++;
    }

    uint64_t FrameManager::GetFrameNumber() const {
        return _currentFrameNumber;
    }

    uint32_t FrameManager::GetCurrentFrameIndex() const {
        return _currentFrameNumber % NB_OVERLAPPING_FRAMES;
    }

    const FrameData FrameManager::GetCurrentFrame() const {
        return GetFrame(GetCurrentFrameIndex());
    }

    const vk::CommandPool FrameManager::GetCurrentCommandPool() const {
        return GetCommandPool(GetCurrentFrameIndex());
    }

    const vk::CommandBuffer FrameManager::GetCurrentCommandBuffer() const {
        return GetCommandBuffer(GetCurrentFrameIndex());
    }

    const vk::Fence FrameManager::GetCurrentRenderFence() const {
        return GetRenderFence(GetCurrentFrameIndex());
    }

    const vk::Semaphore* FrameManager::GetCurrentRenderSemaphore() const {
        return GetRenderSemaphore(GetCurrentFrameIndex());
    }

    const vk::Semaphore* FrameManager::GetCurrentPresentSemaphore() const {
        return GetPresentSemaphore(GetCurrentFrameIndex());
    }
    
}