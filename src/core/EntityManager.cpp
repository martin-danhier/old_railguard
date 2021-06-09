#include "../../include/core/EntityManager.h"
#include <cassert>

#define MIN_FREED_INDICES_BEFORE_REUSE 1024

namespace railguard::core
{

    EntityManager::EntityManager(size_t defaultCapacity)
    {
        _lookupList.reserve(defaultCapacity);
    }

    Entity EntityManager::CreateEntity()
    {
        eid_t indexPart = 0;

        // If enough indices have been freed, reuse the first slot of the queue
        if (_freedIndices.size() >= MIN_FREED_INDICES_BEFORE_REUSE)
        {
            indexPart = _freedIndices.front();
            _freedIndices.pop_front();
        }
        // Else, use a new slot
        else
        {
            _lookupList.push_back(0);
            // The new index part will be the index of the new slot
            indexPart = _lookupList.size() - 1;
            // Make sure it does not go beyond the limit of available indices
            assert(
                indexPart < (static_cast<eid_t>(1) << INDEX_BITS) && "Entity ID index out of range. Too many instanciated entities.");
        }

        // Return the new entity
        return Entity(indexPart, _lookupList[indexPart]);
    }

    void EntityManager::KillEntity(Entity entity)
    {
        eid_t index = entity.GetIndex();

        // When an entity is destroyed, its slot needs to be freed
        _freedIndices.push_back(index);
        // The unique part also needs to be incremented
        // It will overflow when it reaches the maximum, an id could then be used twice
        // But this case is rare, thus we hope that when this occurs, the data about the previous
        // owner of this id will be long gone
        // If this causes a problem, the UNIQUE_BITS value can be increased to increase the time between reuses
        _lookupList[index] += 1;
    }

    bool EntityManager::IsEntityAlive(Entity entity) const
    {
        // Check if the unique id at the index of the entity is the same
        // If not, then this entity was killed and the unique value incremented
        // This allow to quickly check if an entity is alive
        return _lookupList[entity.GetIndex()] == entity.GetUnique();
    }

} // namespace core