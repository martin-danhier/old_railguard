#include "../../include/core/Match.h"
#include <cassert>

namespace railguard::core
{
    Match::Match(size_t i) : _index(i) {}
    size_t Match::GetIndex() const
    {
        // We suppose that there is a result
        assert(HasResult());
        // Else it will underflow
        return _index - 1;
    }
    bool Match::HasResult() const
    {
        return _index > 0;
    }
} // namespace core
