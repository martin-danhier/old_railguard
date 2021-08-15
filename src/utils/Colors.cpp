#include "railguard/utils/Colors.h"

namespace railguard::utils
{

    Color GetColor(float r, float g, float b, float a)
    {
        return Color{r, g, b, a};
    }

    Color GetColorInt(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
    {
        return Color{
            r / 255.0f,
            g / 255.0f,
            b / 255.0f,
            a / 255.0f
        };
    }

    Color GetColorHex(uint32_t rgba){
        return Color{
            ((rgba >> 24) & 0xFF) / 255.0f,
            ((rgba >> 16) & 0xFF) / 255.0f,
            ((rgba >> 8) & 0xFF) / 255.0f,
            (rgba & 0xFF) / 255.0f
        };
    }
} // namespace railguard::utils
