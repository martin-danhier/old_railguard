#include "../../include/core/Match.h"
#include "../../include/utils/AdvancedCheck.h"

#ifdef USE_ADVANCED_CHECKS
// Define error messages
#define NO_RESULT_ERROR "The provided Match has no result. Please use Match::HasResult before calling this method."
#endif

namespace railguard::core
{
    Match::Match(size_t i) : _index(i) {}
    size_t Match::GetIndex() const
    {
        // We suppose that there is a result
        ADVANCED_CHECK(HasResult(), NO_RESULT_ERROR);

        // Else it will underflow
        return _index - 1;
    }
    bool Match::HasResult() const
    {
        return _index > 0;
    }
} // namespace core
