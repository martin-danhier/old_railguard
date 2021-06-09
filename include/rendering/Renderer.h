#pragma once
#include "core/ComponentManager.h"
#include "core/WindowManager.h"

#define VULKAN_HPP_DISPATCH_LOADER_DYNAMIC 1
#include <vulkan/vulkan.hpp>

namespace railguard::rendering
{

    class Renderer : public core::ComponentManager
    {
    private:
        // Vulkan global objects
        vk::Instance _instance{nullptr};
        vk::DebugUtilsMessengerEXT _debugMessenger{nullptr};
        
    public:
        void Init(const core::WindowManager &windowManager);
        explicit Renderer(const size_t defaultComponentCapacity);
        ~Renderer();
        core::ComponentMatch CreateComponent(const core::Entity &entity, int test);
        void DestroyComponent(size_t index) override;
    };

} // namespace railguard::rendering
