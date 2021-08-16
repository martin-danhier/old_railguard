#include "railguard/rendering/init/VulkanInit.h"

#include "railguard/core/WindowManager.h"
#include "railguard/includes/Vulkan.h"
#include "railguard/rendering/Settings.h"
#include "railguard/rendering/init/SwapchainInitInfo.h"
#include "railguard/rendering/init/VulkanInitInfo.h"

#include <VkBootstrap.h>

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
#ifdef USE_VK_VALIDATION_LAYERS
        // Enable validation layers and debug messenger in debug mode
        vkbInstanceBuilder.request_validation_layers(true).enable_validation_layers().use_default_debug_messenger();
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
        *initInfo.instance       = vkbInstance.instance;
        *initInfo.debugMessenger = vkbInstance.debug_messenger;

        // Initialize function pointers for instance
        VULKAN_HPP_DEFAULT_DISPATCHER.init(*initInfo.instance);

        // Get the surface of the SDL window
        *initInfo.surface = initInfo.windowManager.GetVulkanSurface(*initInfo.instance);

        // Select a physical device
        vkb::PhysicalDeviceSelector gpuSelector {vkbInstance};
        auto vkbPhysicalDevice = gpuSelector.set_minimum_version(1, 1)
                                     .set_surface(*initInfo.surface)
                                     .add_required_extension(VK_KHR_SWAPCHAIN_EXTENSION_NAME)
                                     .select()
                                     .value();
        // Get logical device
        vkb::DeviceBuilder deviceBuilder {vkbPhysicalDevice};
        auto vkbDevice = deviceBuilder.build().value();

        // Save devices
        *initInfo.device         = vkbDevice.device;
        *initInfo.physicalDevice = vkbPhysicalDevice.physical_device;

        // Initialize function pointers for device
        VULKAN_HPP_DEFAULT_DISPATCHER.init(*initInfo.device);

        // Get queue
        *initInfo.graphicsQueue       = vkbDevice.get_queue(vkb::QueueType::graphics).value();
        *initInfo.graphicsQueueFamily = vkbDevice.get_queue_index(vkb::QueueType::graphics).value();

        // Get physical device properties
        *initInfo.physicalDeviceProperties = (*initInfo.physicalDevice).getProperties();
    }

    void VulkanInit::InitWindowSwapchain(const SwapchainInitInfo &initInfo)
    {
        // Init swapchain with vk bootstrap
        vkb::SwapchainBuilder swapchainBuilder {initInfo.physicalDevice, initInfo.device, initInfo.surface};
        auto vkbSwapchain = swapchainBuilder.set_desired_format({SWAPCHAIN_FORMAT, SWAPCHAIN_COLOR_SPACE})
                                .set_desired_present_mode(VK_PRESENT_MODE_FIFO_KHR)
                                .set_desired_extent(initInfo.windowExtent.width, initInfo.windowExtent.height)
                                .build()
                                .value();

        // Store swapchain and images
        *initInfo.swapchain            = vkbSwapchain.swapchain;
        *initInfo.swapchainImages      = initInfo.device.getSwapchainImagesKHR(*initInfo.swapchain);
        *initInfo.swapchainImageFormat = vk::Format(vkbSwapchain.image_format);

        // Get image views
        const auto swapchainImageCount = static_cast<uint32_t>(initInfo.swapchainImages->size());
        initInfo.swapchainImageViews->resize(swapchainImageCount);

        for (uint32_t i = 0; i < static_cast<uint32_t>(swapchainImageCount); i++)
        {
            vk::ImageViewCreateInfo createInfo {
                .image    = (*initInfo.swapchainImages)[i],
                .viewType = vk::ImageViewType::e2D,
                .format   = *initInfo.swapchainImageFormat,
                .components {
                    .r = vk::ComponentSwizzle::eIdentity,
                    .g = vk::ComponentSwizzle::eIdentity,
                    .b = vk::ComponentSwizzle::eIdentity,
                    .a = vk::ComponentSwizzle::eIdentity,
                },
                .subresourceRange {
                    .aspectMask     = vk::ImageAspectFlagBits::eColor,
                    .baseMipLevel   = 0,
                    .levelCount     = 1,
                    .baseArrayLayer = 0,
                    .layerCount     = 1,
                },
            };

            (*initInfo.swapchainImageViews)[i] = initInfo.device.createImageView(createInfo);
        }

        // TODO depth image

        // Create framebuffers

#define ATTACHMENTS_COUNT 1
        vk::ImageView attachments[ATTACHMENTS_COUNT];
        // TODO insert depth image view when done

        // Define the init info
        vk::FramebufferCreateInfo framebufferCreateInfo {
            .renderPass      = initInfo.renderPass,
            .attachmentCount = ATTACHMENTS_COUNT,
            .pAttachments    = attachments,
            .width           = initInfo.windowExtent.width,
            .height          = initInfo.windowExtent.height,
            .layers          = 1,
        };

        initInfo.swapchainFramebuffers->resize(swapchainImageCount);

        // Create a framebuffer for each image
        for (uint32_t i = 0; i < swapchainImageCount; i++)
        {
            attachments[0]                       = (*initInfo.swapchainImageViews)[i];
            (*initInfo.swapchainFramebuffers)[i] = initInfo.device.createFramebuffer(framebufferCreateInfo);
        }
    }
} // namespace railguard::rendering::init