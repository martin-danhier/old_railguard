#include "../../include/utils/Colors.h"

namespace railguard::utils
{
    std::array<float, 4> GetColor(float r, float g, float b, float a)
    {
        return std::array<float, 4>{r, g, b, a};
    }

    std::array<float, 4> GetColorInt(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
    {
        return std::array<float, 4>{
            r / 255.0f,
            g / 255.0f,
            b / 255.0f,
            a / 255.0f
        };
    }

    std::array<float, 4> GetColorHex(uint32_t rgba){
        return std::array<float, 4>{
            ((rgba >> 24) & 0xFF) / 255.0f,
            ((rgba >> 16) & 0xFF) / 255.0f,
            ((rgba >> 8) & 0xFF) / 255.0f,
            (rgba & 0xFF) / 255.0f
        };
    }
} // namespace railguard::utils
