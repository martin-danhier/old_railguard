#include "../../include/utils/DebugAssert.h"

#ifndef NDEBUG

#include <iostream>

namespace railguard::utils
{
    void DebugAssert(bool condition, const std::string &message)
    {
        // The condition must be true
        if (condition == false)
        {
            std::cerr << "\n[Assertion Error]\n"
                      << message << std::endl;
            throw std::runtime_error(message);
        }
    }
}

#endif
