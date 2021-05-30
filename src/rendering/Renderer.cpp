#include "rendering/Renderer.h"

namespace railguard::rendering
{

    Renderer::Renderer(const size_t defaultComponentCapacity)
        : ComponentManager(defaultComponentCapacity)
    {
        // Init vectors
    }

    core::ComponentMatch Renderer::CreateComponent(const core::Entity &entity, int test) {
        // TODO insert values
        

        // Run boilerplate for entity management
        return this->RegisterComponent(entity);
    }

    void Renderer::DestroyComponent(size_t index) {
        // Run boilerplate deletion
        core::ComponentManager::DestroyComponent(index);

        // Move the last item of vectors to the destroyed index
    }
    
} // namespace railguard::rendering
