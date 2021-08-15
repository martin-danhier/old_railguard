#include "railguard/utils/GetError.h"

namespace railguard::utils
{
    std::string GetError()
    {
        char msg[256];
        strerror_s(msg, 256, errno);
        return std::string(msg);
    }
} // namespace railguard::utils
