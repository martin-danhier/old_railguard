#pragma once
#include "../core/ComponentManager.h"
#include "../core/WindowManager.h"

#include "init/VulkanInit.h"

namespace railguard::rendering
{

    class Renderer : public core::ComponentManager
    {
    private:
        // Vulkan global objects
        vk::Instance _instance{nullptr};
        vk::DebugUtilsMessengerEXT _debugMessenger{nullptr};
        vk::SurfaceKHR _surface{nullptr};
        vk::PhysicalDevice _physicalDevice{nullptr};
        vk::PhysicalDeviceProperties _physicalDeviceProperties{{}};
        vk::Device _device{nullptr};
        vk::Queue _graphicsQueue{nullptr};
        uint32_t _graphicsQueueFamily{0};
        VmaAllocator _allocator{nullptr};

    public:
        void Init(const core::WindowManager &windowManager);
        explicit Renderer(const size_t defaultComponentCapacity);
        ~Renderer();
        core::Match CreateComponent(const core::Entity &entity);
        void DestroyComponent(size_t index) override;
    };

} // namespace railguard::rendering
