#include "../../include/rendering/Settings.h"
#include "../../include/rendering/Renderer.h"
#include "../../include/rendering/init/RenderPassBuilder.h"
#include "../../include/rendering/structs/Storages.h"
#include "../../include/rendering/init/PipelineLayoutBuilder.h"

namespace railguard::rendering
{

	Renderer::Renderer(const core::WindowManager &windowManager)
		: _swapchainCameraManager(1)
	{

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

		// Init frame manager
		_frameManager.Init(_device, _graphicsQueueFamily);

		// Init the render pass for the main swapchain
		_mainRenderPass = init::RenderPassBuilder()
							  .SetPipelineBindPoint(vk::PipelineBindPoint::eGraphics)
							  .AddColorAttachment(init::AttachmentBuilder()
													  .SetFormat(static_cast<vk::Format>(SWAPCHAIN_FORMAT))
													  .ClearOnLoad()
													  .StoreAtEnd()
													  .SetFinalLayout(vk::ImageLayout::ePresentSrcKHR)
													  .Build())
							  .Build(_device);

		// Init swapchain for window
		_swapchainManager.Init(structs::FullDeviceStorage{_device, _physicalDevice}, 1);
		_mainWindowSwapchain = _swapchainManager.CreateWindowSwapchain(_surface, windowManager, _mainRenderPass).GetId();

		// Init shader module manager
		_shaderModuleManager.Init(structs::DeviceStorage{_device}, 5);

		// Init shader effect manager
		_shaderEffectManager.Init(ShaderEffectManagerStorage{_device, _mainRenderPass, &_shaderModuleManager, &windowManager}, 5);

		// Test
		auto vertexModule = _shaderModuleManager.LoadShaderModule(vk::ShaderStageFlagBits::eVertex, "./bin/shaders/triangle.vert.spv").GetId();
		auto fragmentModule = _shaderModuleManager.LoadShaderModule(vk::ShaderStageFlagBits::eFragment, "./bin/shaders/triangle.frag.spv").GetId();
		_triangleEffect = _shaderEffectManager.CreateShaderEffect(init::ShaderEffectInitInfo {
			.pipelineLayout = init::PipelineLayoutBuilder().Build(_device),
			.shaderStages = {vertexModule, fragmentModule}
		}, true).GetId(); // Build the effect after creation


	}

	Renderer::~Renderer()
	{
		// Destroy shader effect manager
		_shaderEffectManager.Clear();
		// Destroy shader module manager
		_shaderModuleManager.Clear();
		// Destroy swapchains
		_swapchainManager.Clear();
		// Destroy main render pass
		_device.destroyRenderPass(_mainRenderPass);
		// Destroy frame manager
		_frameManager.Cleanup();
		// Destroy allocator
		vmaDestroyAllocator(_allocator);
		// Destroy device
		_device.destroy();
		// Destroy surface
		_instance.destroySurfaceKHR(_surface);
// Destroy instance
#ifdef USE_VK_VALIDATION_LAYERS
		_instance.destroyDebugUtilsMessengerEXT(_debugMessenger);
#endif
		_instance.destroy();
	}

	void Renderer::Draw()
	{
	}

} // namespace railguard::rendering
