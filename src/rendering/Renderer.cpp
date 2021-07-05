#include "../../include/rendering/Renderer.h"

namespace railguard::rendering
{

	void Renderer::Init(const core::WindowManager& windowManager)
	{
		// === Init Vulkan ===

		// Init instance
		init::VulkanInitInfo vulkanInitInfo {
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

	}

	Renderer::Renderer(const size_t defaultComponentCapacity)
		: ComponentManager(defaultComponentCapacity)
	{
		// Init vectors
	}

	Renderer::~Renderer()
	{
		// Destroy allocator
		// vmaDestroyAllocator(_allocator);
		// // Destroy device
		// _device.destroy();
		// // Destroy surface
		// _instance.destroySurfaceKHR(_surface);
		// Destroy instance
		_instance.destroyDebugUtilsMessengerEXT(_debugMessenger);
		_instance.destroy();
	}

	core::Match Renderer::CreateComponent(const core::Entity& entity)
	{
		// TODO insert values

		// Run boilerplate for entity management
		return this->RegisterComponent(entity);
	}

	void Renderer::DestroyComponent(size_t index)
	{
		// Run boilerplate deletion
		core::ComponentManager::DestroyComponent(index);

		// Move the last item of vectors to the destroyed index
	}

} // namespace railguard::rendering
