#include "../../include/rendering/Renderer.h"
#include <VkBootstrap.h>

#define VMA_IMPLEMENTATION
#define VMA_STATIC_VULKAN_FUNCTIONS 0
#define VMA_DYNAMIC_VULKAN_FUNCTIONS 0
#include <vk_mem_alloc.h>


VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE


namespace railguard::rendering
{

	void Renderer::Init(const core::WindowManager& windowManager)
	{
		// === Init Vulkan ===

		// Init dynamic loader
		vk::DynamicLoader loader;
		auto vkGetInstanceProcAddr = loader.getProcAddress<PFN_vkGetInstanceProcAddr>("vkGetInstanceProcAddr");
		VULKAN_HPP_DEFAULT_DISPATCHER.init(vkGetInstanceProcAddr);

		// Create instance
		vkb::InstanceBuilder vkbInstanceBuilder;
#ifndef NDEBUG
		// Enable validation layers and debug messenger in debug mode
		vkbInstanceBuilder.request_validation_layers(true).use_default_debug_messenger();
#endif

		// Get required SDL extensions
		auto requiredSDLExtensions = windowManager.GetRequiredVulkanExtensions();

		vkbInstanceBuilder.set_app_name("My wonderful game")
			.set_app_version(0, 1, 0)
			.set_engine_version(0, 1, 0)
			.require_api_version(1, 1, 0)
			.set_engine_name("Railguard");

		// Add sdl extensions
		for (const char* ext : requiredSDLExtensions) {
			vkbInstanceBuilder.enable_extension(ext);
		}

		// Build instance
		auto vkbInstance = vkbInstanceBuilder.build().value();

		// Wrap the instance and debug messenger in vk-hpp handle classes and store
		// them
		_instance = vkbInstance.instance;
		_debugMessenger = vkbInstance.debug_messenger;

		// Initialize function pointers for instance
		VULKAN_HPP_DEFAULT_DISPATCHER.init(_instance);

		// Get the surface of the SDL window
		_surface = windowManager.GetVulkanSurface(_instance);

		// Select a physical device
		vkb::PhysicalDeviceSelector gpuSelector{vkbInstance};
		auto vkbPhysicalDevice = gpuSelector.set_minimum_version(1,1)
											.set_surface(_surface)
											.select()
											.value();
		// Get logical device
		vkb::DeviceBuilder deviceBuilder{vkbPhysicalDevice};
		auto vkbDevice = deviceBuilder.build().value();

		// Save devices
		_device = vkbDevice.device;
		_physicalDevice = vkbPhysicalDevice.physical_device;

		// Initialize function pointers for device
		VULKAN_HPP_DEFAULT_DISPATCHER.init(_device);

		// Get queue
		_graphicsQueue = vkbDevice.get_queue(vkb::QueueType::graphics).value();
		_graphicsQueueFamily = vkbDevice.get_queue_index(vkb::QueueType::graphics).value();

		// Get physical device properties
		_physicalDeviceProperties = _physicalDevice.getProperties();

		// Init allocator
		// Give VMA the functions pointers of vulkan functions
  		// We need to do that since we load them dynamically
  		VmaVulkanFunctions vulkanFunctions{
  		    VULKAN_HPP_DEFAULT_DISPATCHER.vkGetPhysicalDeviceProperties,
  		    VULKAN_HPP_DEFAULT_DISPATCHER.vkGetPhysicalDeviceMemoryProperties,
  		    VULKAN_HPP_DEFAULT_DISPATCHER.vkAllocateMemory,
  		    VULKAN_HPP_DEFAULT_DISPATCHER.vkFreeMemory,
  		    VULKAN_HPP_DEFAULT_DISPATCHER.vkMapMemory,
  		    VULKAN_HPP_DEFAULT_DISPATCHER.vkUnmapMemory,
  		    VULKAN_HPP_DEFAULT_DISPATCHER.vkFlushMappedMemoryRanges,
  		    VULKAN_HPP_DEFAULT_DISPATCHER.vkInvalidateMappedMemoryRanges,
  		    VULKAN_HPP_DEFAULT_DISPATCHER.vkBindBufferMemory,
  		    VULKAN_HPP_DEFAULT_DISPATCHER.vkBindImageMemory,
  		    VULKAN_HPP_DEFAULT_DISPATCHER.vkGetBufferMemoryRequirements,
  		    VULKAN_HPP_DEFAULT_DISPATCHER.vkGetImageMemoryRequirements,
  		    VULKAN_HPP_DEFAULT_DISPATCHER.vkCreateBuffer,
  		    VULKAN_HPP_DEFAULT_DISPATCHER.vkDestroyBuffer,
  		    VULKAN_HPP_DEFAULT_DISPATCHER.vkCreateImage,
  		    VULKAN_HPP_DEFAULT_DISPATCHER.vkDestroyImage,
  		    VULKAN_HPP_DEFAULT_DISPATCHER.vkCmdCopyBuffer,
			VULKAN_HPP_DEFAULT_DISPATCHER.vkGetBufferMemoryRequirements2KHR,
			VULKAN_HPP_DEFAULT_DISPATCHER.vkGetImageMemoryRequirements2KHR,
			VULKAN_HPP_DEFAULT_DISPATCHER.vkBindBufferMemory2KHR,
			VULKAN_HPP_DEFAULT_DISPATCHER.vkBindImageMemory2KHR,
			VULKAN_HPP_DEFAULT_DISPATCHER.vkGetPhysicalDeviceMemoryProperties2KHR,
  		};
  		VmaAllocatorCreateInfo allocatorInfo{
  		    .physicalDevice = _physicalDevice,
  		    .device = _device,
  		    .pVulkanFunctions = &vulkanFunctions,
  		    .instance = _instance,
  		};
  		vmaCreateAllocator(&allocatorInfo, &_allocator);
	}

	Renderer::Renderer(const size_t defaultComponentCapacity)
		: ComponentManager(defaultComponentCapacity)
	{
		// Init vectors
	}

	Renderer::~Renderer()
	{
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

	core::ComponentMatch Renderer::CreateComponent(const core::Entity& entity)
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
