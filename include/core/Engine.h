#pragma once

#include "EntityManager.h"
#include "WindowManager.h"
#include "../rendering/Renderer.h"
#include <cmath>

namespace railguard::core
{

    class Engine
    {
    private:
        EntityManager _entityManager;
        WindowManager _windowManager;
        rendering::Renderer _renderer;
        double_t _deltaTime;

    public:
        explicit Engine();
        ~Engine();

        void RunMainLoop();

        void OnWindowResized(const vk::Extent2D &newSize);

    };

} // namespace railguard::core