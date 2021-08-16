#include "railguard/utils/GetError.h"

#ifdef __GNUC__
    #include <cstring>
#endif

namespace railguard::utils
{
    std::string GetError()
    {
#ifdef __GNUC__
        return std::strerror(errno);
#else
        char msg[256];
        strerror_s(msg, 256, errno);
        return std::string(msg);
#endif
    }
} // namespace railguard::utils
