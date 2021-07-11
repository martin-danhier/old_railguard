#include "../../include/rendering/Renderer.h"
#include "../../include/rendering/init/RenderPassBuilder.h"

namespace railguard::rendering
{

	Renderer::Renderer(const core::WindowManager &windowManager)
		: _swapchainCameraManager(1)
	{
		// === Init Vulkan ===

		// Init instance
		init::VulkanInitInfo vulkanInitInfo{
			.windowManager = windowManager,
			.instance = &_instance,
			.debugMessenger = &_debugMessenger,
			.surface = &_surface,
			.physicalDevice = &_physicalDevice,
			.physicalDeviceProperties = &_physicalDeviceProperties,
			.device = &_device,
			.graphicsQueue = &_graphicsQueue,
			.graphicsQueueFamily = &_graphicsQueueFamily,
			.allocator = &_allocator,
		};
		init::VulkanInit::InitVulkan(vulkanInitInfo);

		// Init swapchain for window
		_swapchainManager.Init(_device, _physicalDevice, 1);
		_mainWindowSwapchain = _swapchainManager.CreateWindowSwapchain(_surface, windowManager);
		// Get the format of that swapchain
		auto mainWindowSwapchainFormat = _swapchainManager.GetSwapchainImageFormat(_swapchainManager.LookupId(_mainWindowSwapchain));

		// Init frame manager
		_frameManager.Init(_device, _graphicsQueueFamily);

		// Init the render pass for the main swapchain
		_mainRenderPass = init::RenderPassBuilder()
							  .SetPipelineBindPoint(vk::PipelineBindPoint::eGraphics)
							  .AddColorAttachment(init::AttachmentBuilder()
													  .SetFormat(mainWindowSwapchainFormat)
													  .ClearOnLoad()
													  .StoreAtEnd()
													  .SetFinalLayout(vk::ImageLayout::ePresentSrcKHR)
													  .Build())
							  .Build(_device);
	}

	Renderer::~Renderer()
	{
		// Destroy main render pass
		_device.destroyRenderPass(_mainRenderPass);
		// Destroy frame manager
		_frameManager.Cleanup();
		// Destroy swapchains
		_swapchainManager.Clear();
		// Destroy allocator
		vmaDestroyAllocator(_allocator);
		// Destroy device
		_device.destroy();
		// Destroy surface
		_instance.destroySurfaceKHR(_surface);
		// Destroy instance
		_instance.destroyDebugUtilsMessengerEXT(_debugMessenger);
		_instance.destroy();
	}

	void Renderer::Draw()
	{
	}

} // namespace railguard::rendering
