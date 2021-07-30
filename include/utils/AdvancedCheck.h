#pragma once

#ifdef USE_ADVANCED_CHECKS

#include <string>

// Call the advanced check method if the USE_ADVANCED_CHECKS variable is set

/**
 * @brief In debug mode, throws an error with MSG as message if COND is false. Doesn't do anything in release mode. (Currently in debug mode)
 */
#define ADVANCED_CHECK(COND, MSG) railguard::utils::AdvancedCheck(COND, MSG)

namespace railguard::utils
{
    /**
     * @brief Throws an error if the condition is false.
     *
     * @param condition The condition that must be true.
     * @param message The message of the error thrown if the condition is false.
     */
    void AdvancedCheck(bool condition, const std::string &message);
}

#else

// The macro does nothing in release mode
// It exists to avoid needing to wrap every call in ifndef checks

/**
 * @brief In debug mode, throws an error with MSG as message if COND is false. Doesn't do anything in release mode. (Currently in release mode)
 */
#define ADVANCED_CHECK(COND, MSG)

#endif