#pragma once

// Forward declaration of what we need
namespace vk
{
    class Extent2D;
} // namespace vk

namespace railguard
{
    namespace rendering
    {
        class Renderer;
    } // namespace rendering
    
    namespace core
    {
        /**
         * @brief Central class of the engine. It manages the most central aspects of it.
         */
        class Engine
        {
          private:
            class EntityManager *_entityManager = nullptr;
            class WindowManager *_windowManager = nullptr;
            rendering::Renderer *_renderer = nullptr;
            double _deltaTime = 0;

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

    } // namespace core
} // namespace railguard