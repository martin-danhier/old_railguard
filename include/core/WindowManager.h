#include "SDL2/SDL.h"
#include "EventSender.h"
#include <string>

namespace railguard::core
{
    struct Extent2D
    {
        uint32_t width;
        uint32_t height;
    };

    class WindowManager
    {
    private:
        /**
         * @brief Handle for the main SDL window
         */
        SDL_Window *_window{nullptr};
        uint32_t _width{0};
        uint32_t _height{0};
        std::string _title{""};
        // Event thrown when the window is resized
        EventSender<Extent2D> _resizeEvent;

    public:
        WindowManager(int defaultWidth, int defaultHeight, const std::string &defaultTitle);
        void CreateWindow();
        void DestroyWindow();
        uint64_t GetPerformanceCounter();
        uint64_t GetPerformanceFrequency();
        bool HandleEvents();

        EventSender<Extent2D> &ResizeEvent();
    };
} // namespace railguard::core
