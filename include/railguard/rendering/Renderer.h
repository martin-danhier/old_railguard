#pragma once

#include "railguard/includes/Vulkan.h"
#include "railguard/rendering/Ids.h"

namespace railguard
{
    namespace core
    {
        class WindowManager;
    }
    namespace rendering
    {
        /**
         * @brief Main class of the rendering system.
         *
         * The renderer handles the entirety of the rendering process.
         * It manages Vulkan object handles, internal managers and components used for the rendering.
         *
         * However, it doesn't manage the window itself, and only connects to one that is provided at initialization.
         */
        class Renderer
        {
          private:
            // Vulkan global objects
            vk::Instance _instance                                 = nullptr;
            vk::DebugUtilsMessengerEXT _debugMessenger             = nullptr;
            vk::SurfaceKHR _surface                                = nullptr;
            vk::PhysicalDevice _physicalDevice                     = nullptr;
            vk::PhysicalDeviceProperties _physicalDeviceProperties = {};
            vk::Device _device                                     = nullptr;
            vk::Queue _graphicsQueue                               = nullptr;
            uint32_t _graphicsQueueFamily                          = 0;
            vk::RenderPass _mainRenderPass                         = nullptr;

            // Other internal variables
            swapchain_id_t _mainWindowSwapchain = 0;
            vk::Extent2D _windowExtent;

            // Various managers for core objects
            class SwapchainManager *_swapchainManager               = nullptr;
            class SwapchainCameraManager *_swapchainCameraManager   = nullptr;
            class FrameManager *_frameManager                       = nullptr;
            class ShaderModuleManager *_shaderModuleManager         = nullptr;
            class ShaderEffectManager *_shaderEffectManager         = nullptr;
            class MaterialTemplateManager *_materialTemplateManager = nullptr;
            class MaterialManager *_materialManager                 = nullptr;
            class ModelManager *_modelManager                       = nullptr;
            class RenderStageManager *_renderStageManager           = nullptr;
            class AllocationManager *_allocationManager             = nullptr;

            // Test
            shader_effect_id_t _triangleEffect;

          public:
            explicit Renderer(const core::WindowManager &windowManager);

            /**
             * @brief Renders an image for every camera
             */
            void Draw();

            // === Components ===

            SwapchainCameraManager *GetSwapchainCameraManager();

            ~Renderer();
        };

    } // namespace rendering
} // namespace railguard