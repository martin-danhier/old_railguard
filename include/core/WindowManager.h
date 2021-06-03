#include "SDL2/SDL.h"
#include "EventSender.h"
#include <string>

namespace railguard::core
{
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
        EventSender<uint32_t> _resizeEvent;

    public:
        WindowManager(int defaultWidth, int defaultHeight, const std::string &defaultTitle);
        void CreateWindow();
        void DestroyWindow();
        uint64_t GetPerformanceCounter();
        uint64_t GetPerformanceFrequency();
        bool HandleEvents();
    };
} // namespace railguard::core
