#include "core/ComponentMatch.h"
#include <cassert>

namespace railguard::core
{
    ComponentMatch::ComponentMatch(size_t i) : _index(i) {}
    size_t ComponentMatch::GetIndex() const
    {
        // We suppose that there is a result
        assert(HasResult());
        // Else it will underflow
        return _index - 1;
    }
    bool ComponentMatch::HasResult() const
    {
        return _index > 0;
    }
} // namespace core
