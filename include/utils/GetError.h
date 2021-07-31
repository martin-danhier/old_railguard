#pragma once
#include <string>

namespace railguard::utils
{
    /**
     * @brief Get the error message corresponding to the current value of errno.
     *
     * @return std::string The error message.
     */
    std::string GetError();

} // namespace railguard::utils
