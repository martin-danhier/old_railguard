#pragma once

#include "core/EntityManager.h"
#include "WindowManager.h"
#include <cmath>

namespace railguard::core
{

    class Engine
    {
    private:
        EntityManager _entityManager;
        WindowManager _windowManager;
        double_t _deltaTime;

    public:
        explicit Engine();
        ~Engine();

        void RunMainLoop();

        void OnWindowResized(const Extent2D &newSize);

    };

} // namespace railguard::core