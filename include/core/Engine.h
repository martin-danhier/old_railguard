#pragma once

#include "EntityManager.h"
#include "WindowManager.h"
#include "../rendering/Renderer.h"
#include <cmath>

namespace railguard::core
{

    /**
     * @brief Central class of the engine. It manages the most central aspects of it.
     */
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

        /**
         * @brief Starts the main loop of the engine.
         *
         * The function closes when the execution should be terminated (for example, the game was stopped by the user).
         */
        void RunMainLoop();

        /**
         * @brief Event handler for the window resize event.
         *
         * @param newSize New size of the window.
         */
        void OnWindowResized(const vk::Extent2D &newSize);

    };

} // namespace railguard::core