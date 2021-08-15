#pragma once

/**
 * @file Defines the camera mode enum.
 */

namespace railguard::rendering::enums
{
    /**
     * @brief The camera mode defines the projection matrix that will be used by a camera.
     * @see The description of the variants for details about them.
     */
    enum class CameraMode : uint32_t
    {
        /**
         * @brief Far objects will appear the same size as close ones.
         *
         * Useful for development.
         */
        Orthographic,
        /**
         * @brief Projection matrix approximating real life: far objects will appear smaller.
         *
         * This is the mode that is used in almost all cases.
         */
        Perspective
    };

} // namespace railguard::rendering::enums
