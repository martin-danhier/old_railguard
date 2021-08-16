#pragma once
#include <railguard/utils/IntegralConcept.h>
#include <cstddef>

namespace railguard::core
{
    // Represents an index in a component manager.
    // Created for example by calling ComponentManager.FindComponentOfEntity or by creating a component
    // Contains either the index of the corresponding component or nothing (in case no component was found)
    // Thus, the HasResult function should be called before the GetIndex value is used
    class Match
    {
    protected:
        // Index of the corresponding component in the manager + 1
        // Is 0 when the entity does not have any component
        size_t _index = 0;

    public:
        explicit Match(size_t index = 0);

        // Returns true if a component was found, false otherwise
        [[nodiscard]] bool HasResult() const;
        // Returns the index. Fails if it does not have any result
        [[nodiscard]] size_t GetIndex() const;
    };

    // Take the type of the original ID as a template parameter.
    // We use concepts to force it to be an integral type
    template <utils::integral T>
    class CompleteMatch : public Match
    {
    private:
        T _id;

    public:
        CompleteMatch(size_t index, T id) : Match(index), _id(id) {}

        [[nodiscard]] T GetId() const
        {
            return _id;
        }
        /**
         * @brief Strips away the original id
         * @return Match
         */
        [[nodiscard]] Match ToMatch() const
        {
            return Match(_index);
        }
    };
}