#include "../../include/utils/AdvancedCheck.h"

#ifdef USE_ADVANCED_CHECKS

#include <iostream>

namespace railguard::utils
{
    void AdvancedCheck(bool condition, const std::string &message)
    {
        // The condition must be true
        if (condition == false)
        {
            std::cerr << "\n[Error]\n"
                      << message << std::endl;
            throw std::runtime_error(message);
        }
    }
}

#endif
