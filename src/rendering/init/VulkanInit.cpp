#define VMA_IMPLEMENTATION
#define VMA_STATIC_VULKAN_FUNCTIONS 0
#define VMA_DYNAMIC_VULKAN_FUNCTIONS 0
#include <vk_mem_alloc.h>
#include "../../include/rendering/init/VulkanInit.h"
#include <VkBootstrap.h>

VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE

namespace railguard::rendering::init
{
	void VulkanInit::InitVulkan(const VulkanInitInfo &initInfo)
	{
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
		auto requiredSDLExtensions = initInfo.windowManager.GetRequiredVulkanExtensions();

		vkbInstanceBuilder.set_app_name("My wonderful game")
			.set_app_version(0, 1, 0)
			.set_engine_version(0, 1, 0)
			.require_api_version(1, 1, 0)
			.set_engine_name("Railguard");

		// Add sdl extensions
		for (const char *ext : requiredSDLExtensions)
		{
			vkbInstanceBuilder.enable_extension(ext);
		}

		// Build instance
		auto vkbInstance = vkbInstanceBuilder.build().value();

		// Wrap the instance and debug messenger in vk-hpp handle classes and store
		// them
		*initInfo.instance = vkbInstance.instance;
		*initInfo.debugMessenger = vkbInstance.debug_messenger;

		// Initialize function pointers for instance
		VULKAN_HPP_DEFAULT_DISPATCHER.init(*initInfo.instance);

		// Get the surface of the SDL window
		*initInfo.surface = initInfo.windowManager.GetVulkanSurface(*initInfo.instance);

		// Select a physical device
		vkb::PhysicalDeviceSelector gpuSelector{vkbInstance};
		auto vkbPhysicalDevice = gpuSelector.set_minimum_version(1, 1)
									 .set_surface(*initInfo.surface)
									 .select()
									 .value();
		// Get logical device
		vkb::DeviceBuilder deviceBuilder{vkbPhysicalDevice};
		auto vkbDevice = deviceBuilder.build().value();

		// Save devices
		*initInfo.device = vkbDevice.device;
		*initInfo.physicalDevice = vkbPhysicalDevice.physical_device;

		// Initialize function pointers for device
		VULKAN_HPP_DEFAULT_DISPATCHER.init(*initInfo.device);

		// Get queue
		*initInfo.graphicsQueue = vkbDevice.get_queue(vkb::QueueType::graphics).value();
		*initInfo.graphicsQueueFamily = vkbDevice.get_queue_index(vkb::QueueType::graphics).value();

		// Get physical device properties
		*initInfo.physicalDeviceProperties = (*initInfo.physicalDevice).getProperties();

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
			.physicalDevice = *initInfo.physicalDevice,
			.device = *initInfo.device,
			.pVulkanFunctions = &vulkanFunctions,
			.instance = *initInfo.instance,
		};
		vmaCreateAllocator(&allocatorInfo, initInfo.allocator);
	}

	void InitSwapchain(const SwapchainInitInfo &initInfo)
	{
		// Get window extent
		auto windowExtent = initInfo.windowManager.GetWindowExtent();

		// Init swapchain with vk bootstrap
		vkb::SwapchainBuilder swapchainBuilder { initInfo.physicalDevice, initInfo.device, initInfo.surface };
		auto vkbSwapchain = swapchainBuilder.use_default_format_selection()
			.set_desired_present_mode(VK_PRESENT_MODE_FIFO_KHR)
			.set_desired_extent(windowExtent.width, windowExtent.height)
			.build()
			.value();

		// Store swapchain and images
		
	}
}