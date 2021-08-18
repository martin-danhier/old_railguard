#include <railguard/core/WindowManager.h>
#include <railguard/rendering/AllocationManager.h>
#include <railguard/rendering/FrameManager.h>
#include <railguard/rendering/MaterialManager.h>
#include <railguard/rendering/MaterialTemplateManager.h>
#include <railguard/rendering/ModelManager.h>
#include <railguard/rendering/RenderStageManager.h>
#include <railguard/rendering/Renderer.h>
#include <railguard/rendering/ShaderEffectManager.h>
#include <railguard/rendering/ShaderModuleManager.h>
#include <railguard/rendering/SwapchainCameraManager.h>
#include <railguard/rendering/SwapchainManager.h>
#include <railguard/rendering/enums/ShaderEffectKind.h>
#include <railguard/rendering/init/PipelineLayoutBuilder.h>
#include <railguard/rendering/init/RenderPassBuilder.h>
#include <railguard/rendering/init/ShaderEffectInitInfo.h>
#include <railguard/rendering/init/VulkanInit.h>
#include <railguard/rendering/init/VulkanInitInfo.h>
#include <railguard/rendering/structs/CameraRenderInfo.h>
#include <railguard/utils/Colors.h>

namespace railguard::rendering
{
    Renderer::Renderer(const core::WindowManager &windowManager)
        : _instance(nullptr),
          _device(nullptr),
          _graphicsQueue(nullptr),
          _surface(nullptr),
          _physicalDevice(nullptr),
          _graphicsQueueFamily(0),
          _debugMessenger(nullptr)
    {
        // Save current extent
        _windowExtent = windowManager.GetWindowExtent();

        // === Init Vulkan and internal managers ===

        // Init instance
        init::VulkanInitInfo vulkanInitInfo {
            .windowManager            = windowManager,
            .instance                 = &_instance,
            .debugMessenger           = &_debugMessenger,
            .surface                  = &_surface,
            .physicalDevice           = &_physicalDevice,
            .physicalDeviceProperties = &_physicalDeviceProperties,
            .device                   = &_device,
            .graphicsQueue            = &_graphicsQueue,
            .graphicsQueueFamily      = &_graphicsQueueFamily,
        };
        init::VulkanInit::InitVulkan(vulkanInitInfo);

        // Init frame manager
        _frameManager = new FrameManager(_device, _graphicsQueueFamily);

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
        _swapchainManager    = new SwapchainManager({_device, _physicalDevice, _frameManager}, 1);
        _mainWindowSwapchain = _swapchainManager->CreateWindowSwapchain(_surface, windowManager, _mainRenderPass).GetId();

        // Init shader module manager
        _shaderModuleManager = new ShaderModuleManager({_device}, 5);

        // Init shader effect manager
        _shaderEffectManager = new ShaderEffectManager({_device, _mainRenderPass, _shaderModuleManager, &windowManager}, 5);

        // Init material system
        _materialTemplateManager = new MaterialTemplateManager({_shaderEffectManager}, 3);
        _materialManager         = new MaterialManager({_materialTemplateManager, _shaderEffectManager}, 10);
        _modelManager            = new ModelManager({_materialManager}, 30);

        // Init allocation manager
        _allocationManager = new AllocationManager(_physicalDevice, _device, _instance);

        // Init passes
        // A pass defines a shader configuration
        _renderStageManager =
            new RenderStageManager({enums::ShaderEffectKind::Forward}, _materialManager, _modelManager, _allocationManager);

        // === Init components ===

        // Init cameras
        _swapchainCameraManager = new SwapchainCameraManager(SwapchainCameraManagerStorage {_swapchainManager}, 1);

        // === Test ===

        auto vertexModule =
            _shaderModuleManager->LoadShaderModule(vk::ShaderStageFlagBits::eVertex, "./bin/shaders/triangle.vert.spv").GetId();
        auto fragmentModule =
            _shaderModuleManager->LoadShaderModule(vk::ShaderStageFlagBits::eFragment, "./bin/shaders/triangle.frag.spv").GetId();
        _triangleEffect = _shaderEffectManager
                              ->CreateShaderEffect(
                                  init::ShaderEffectInitInfo {
                                      .pipelineLayout = init::PipelineLayoutBuilder::Build(_device),
                                      .shaderStages   = {vertexModule, fragmentModule},
                                      .effectKind     = enums::ShaderEffectKind::Forward,
                                  },
                                  true)
                              .GetId(); // Build the effect after creation
                                        // Add a camera
        auto baseMaterialTemplate = _materialTemplateManager->CreateMaterialTemplate({_triangleEffect}).GetId();
        auto baseMaterial         = _materialManager->CreateMaterial(baseMaterialTemplate).GetId();
        auto triangleModel        = _modelManager->CreateModel(baseMaterial);
    }

    Renderer::~Renderer()
    {
        // Wait for all fences
        _frameManager->WaitForAllFences();

        // Destroy camera manager
        delete _swapchainCameraManager;
        _swapchainCameraManager = nullptr;

        // Destroy stage manager
        delete _renderStageManager;
        _renderStageManager = nullptr;
        // Destroy allocation manager
        delete _allocationManager;
        _allocationManager = nullptr;
        // Destroy material system
        delete _modelManager;
        _modelManager = nullptr;
        delete _materialManager;
        _materialManager = nullptr;
        delete _materialTemplateManager;
        _materialTemplateManager = nullptr;
        // Destroy shader effect manager
        delete _shaderEffectManager;
        _shaderEffectManager = nullptr;
        // Destroy shader module manager
        delete _shaderModuleManager;
        _shaderModuleManager = nullptr;
        // Destroy swapchains
        delete _swapchainManager;
        _swapchainManager = nullptr;
        // Destroy main render pass
        _device.destroyRenderPass(_mainRenderPass);
        // Destroy frame manager
        delete _frameManager;
        _frameManager = nullptr;
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
        // Wait for fences
        _frameManager->WaitForCurrentFence();

        // Update render stages cache
        _renderStageManager->UpdateCache();

        // Get render infos from cameras
        std::vector<structs::CameraRenderInfo> cameraRenderInfos = _swapchainCameraManager->GetRenderInfos(_mainRenderPass);

        // Begin recording of commands
        auto cmd = _frameManager->BeginRecording();

        // Each active camera returned a render info containing all the data we need.
        // We need to render to all of them.
        for (auto renderInfo : cameraRenderInfos)
        {
            // We do a render pass for each camera
            cmd.beginRenderPass(renderInfo.renderPassBeginInfo, vk::SubpassContents::eInline);

            _renderStageManager->DrawFromCache(cmd);

            cmd.endRenderPass();
        }

        // End recording of commands
        _frameManager->EndRecordingAndSubmit(_graphicsQueue);

        // Present used swapchain images
        _swapchainManager->PresentUsedImages(_graphicsQueue);

        // Increase the number of frames drawn
        _frameManager->FinishFrame();
    }

    SwapchainCameraManager *Renderer::GetSwapchainCameraManager()
    {
        return _swapchainCameraManager;
    }

} // namespace railguard::rendering
