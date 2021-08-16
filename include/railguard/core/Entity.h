#ifndef ENTITY_H
#define ENTITY_H

#include <cstdint>
#include <cstddef>

namespace railguard::core
{
    // === Config values for the ids ===

    // Type used to represent a full Entity id.
    //
    // A typedef is used to allow better maintainability:
    // If the size of the integer type needs to be increased in the future,
    // all the function handling entities will instantly be updated with the new size.
    typedef uint64_t eid_t;
    // Type used to represent the unique part of an Entity id.
    // It needs to be large enough to fit in every possible unique id,
    // but as small as possible to avoid taking not needed space
    typedef uint8_t unique_eid_t;

    // Number of bits used for the "unique" part of an entity id
    //
    // The "unique part" is used to differentiate entities created at the same index
    // The number of possible unique ids defines the maximum number of entities that can be created in a slot
    // without looping.
    // Note: these are the ids that will be stored in the table. That means that the smaller the unique part is,
    // the less space the entity table will take, and the more ids will fit in the cache.
    //
    // Formula: 2^UNIQUE_BITS
    constexpr uint8_t UNIQUE_BITS = 8;
    // Number of bits used for the "index" part of an entity id
    //
    // The "index part" is used to index an id in the lookup table.
    // The number of possible indexes defines the maximum number of simultaneous alive entities.
    //
    // Formula: 2^INDEX_BITS
    constexpr uint8_t INDEX_BITS = 56;

    // Static assert to prevent mis configuration of the above parameters
    // They must always sum up to the the size of the entity id
    // If in the future, the id must be divided in more than 2 parts, this assertion will need to be
    // updated.
    static_assert(
        static_cast<size_t>(UNIQUE_BITS + INDEX_BITS) == sizeof(eid_t) * 8,
        "The unique and index bits must sum up to the length of the type used to represent Entity ids.");
    // Check if the unique_eid_t type can hold every possible unique id
    static_assert(
        UNIQUE_BITS <= sizeof(unique_eid_t) * 8,
        "The unique_eid_t type must be large enough to fit in every possible unique part of an Entity id.");

    // An entity is an ID that can be used to represent a "thing" in the Engine, like a player, a bullet,
    // a tree, a sound effect, etc.
    // Once created, an id can be used to create components, which represent a part of the logic and data linked to that entity
    struct Entity
    {
        // Id of the entity itself
        eid_t eid;

        explicit Entity(eid_t id);
        explicit Entity(eid_t index, unique_eid_t unique);

        // Returns the index part of this entity
        [[nodiscard]] eid_t GetIndex() const;
        // Returns the unique part of this entity
        [[nodiscard]] unique_eid_t GetUnique() const;
        // Comparison operators between two entities
        bool operator==(const Entity &other) const;
        bool operator==(const Entity &&other) const;
        bool operator!=(const Entity &other) const;
        bool operator!=(const Entity &&other) const;

        // Pretty prints an entity when placed in a cout
        //        friend std::ostream &operator<<(std::ostream &os, const Entity &rhs);
    };

} // namespace railguard::core

#endif // ENTITY_H
