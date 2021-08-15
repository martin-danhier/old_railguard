#include "railguard/core/Entity.h"
#include <ostream>
#include <cinttypes>

namespace railguard::core
{

    Entity::Entity(eid_t id)
    {
        eid = id;
    }

    Entity::Entity(eid_t index, unique_eid_t unique)
    {
        // Generate the id by shifting the index bits to the left and adding the unique ones in the created space
        eid = (index << UNIQUE_BITS) | unique;
    }

    bool Entity::operator==(const Entity &other) const
    {
        return eid == other.eid;
    }
    bool Entity::operator==(const Entity &&other) const
    {
        return eid == other.eid;
    }

    bool Entity::operator!=(const Entity &other) const
    {
        return eid != other.eid;
    }
    bool Entity::operator!=(const Entity &&other) const
    {
        return eid != other.eid;
    }

    eid_t Entity::GetIndex() const
    {
        // We want the first INDEX_BITS bits from the left
        // So we need to remove whatever is on the right and move the bits there
        // A shift is perfect
        constexpr uint8_t shift = sizeof(eid_t) * 8 - INDEX_BITS;
        return eid >> shift;
    }

    unique_eid_t Entity::GetUnique() const
    {
        // We need to keep the first UNIQUE_BITS bits from the right
        // We can use a mask to select the bits that interest us
        constexpr unique_eid_t mask = (1 << UNIQUE_BITS) - 1;
        return eid & mask;
    }
//    std::ostream &operator<<(std::ostream &os, const Entity &rhs)
//    {
//        // Pretty print the entity
//        os << "Entity " << rhs.eid << " (index: " << rhs.GetIndex() << ", unique: " << + rhs.GetUnique() << ")";
//        return os;
//    }

} // namespace core