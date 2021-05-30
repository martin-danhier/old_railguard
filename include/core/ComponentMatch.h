#pragma once
#include <cstddef>

namespace railguard::core {
    // Represents an index in a component manager.
    // Created for example by calling ComponentManager.FindComponentOfEntity or by creating a component
    // Contains either the index of the corresponding component or nothing (in case no component was found)
    // Thus, the HasResult function should be called before the GetIndex value is used
    class ComponentMatch
    {
    private:
        // Index of the corresponding component in the manager + 1
        // Is 0 when the entity does not have any component
        size_t _index;

    public:
        explicit ComponentMatch(size_t index);

        // Returns true if a component was found, false otherwise
        [[nodiscard]] bool HasResult() const;
        // Returns the index. Fails if it does not have any result
        [[nodiscard]] size_t GetIndex() const;

    };
}