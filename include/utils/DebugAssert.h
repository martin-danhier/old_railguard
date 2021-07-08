#pragma once

#ifndef NDEBUG

#include <string>

// In debug mode, call the debug assert method

/**
 * @brief In debug mode, throws an error with MSG as message if COND is false. Doesn't do anything in release mode. (Currently in debug mode)
 */
#define DEBUG_ASSERT(COND, MSG) railguard::utils::DebugAssert(COND, MSG)

namespace railguard::utils
{
    /**
     * @brief Throws an error if the condition is false.
     *
     * @param condition The condition that must be true.
     * @param message The message of the error thrown if the condition is false.
     */
    void DebugAssert(bool condition, const std::string &message);
}

#else

// The macro does nothing in release mode
// It exists to avoid needing to wrap every call in ifndef checks

/**
 * @brief In debug mode, throws an error with MSG as message if COND is false. Doesn't do anything in release mode. (Currently in release mode)
 */
#define DEBUG_ASSERT(COND, MSG)

#endif