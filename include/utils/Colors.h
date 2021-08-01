#pragma once

#include <array>
#include <cinttypes>

namespace railguard::utils
{
    /**
     * @brief R, G, B, A color format. Each value is a float between 0 and 1.
     */
    typedef std::array<float, 4> Color;

    /**
     * @brief Returns the array representation of the given color.
     *
     * @param r Red component between 0 and 1
     * @param g Green component between 0 and 1
     * @param b Blue component between 0 and 1
     * @param a Alpha component between 0 and 1
     * @return std::array<float, 4> The array representation of the color, used for example for clear values.
     */
    std::array<float, 4> GetColor(float r, float g, float b, float a = 1.0f);
    /** @brief Returns the array representation of the given color.
     *
     * @param r Red component between 0 and 255
     * @param g Green component between 0 and 255
     * @param b Blue component between 0 and 255
     * @param a Alpha component between 0 and 255
     * @return std::array<float, 4> The array representation of the color, used for example for clear values.
     */
    std::array<float, 4> GetColorInt(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255);
    /**
     * @brief Returns the array representation of the given color.
     *
     * @note Slower than the overload where each rgba value is given separately.
     *
     * @param rgba Representation in a single value, e.g. 0xff0000ff
     * @return std::array<float, 4> The array representation of the color, used for example for clear values.
     */
    std::array<float, 4> GetColorHex(uint32_t rgba);
} // namespace railguard::utils
