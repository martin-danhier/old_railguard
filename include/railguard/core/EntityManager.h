#pragma once

#include "railguard/core/Entity.h"
#include <vector>
#include <deque>

namespace railguard::core
{

    class EntityManager
    {

    private:
        // Stores the unique ids of the alive entities
        std::vector<unique_eid_t> _lookupList;
        // Stores the indices that were freed
        std::deque<eid_t> _freedIndices;

    public:
        // Inits the entity manager
        // The defaultCapacity parameter defines how many entities will be able to be stored without resizing the array
        explicit EntityManager(size_t defaultCapacity);
        // Create and register a new entity
        Entity CreateEntity();
        // Kills the given entity and frees its id slot
        void KillEntity(Entity entity);
        // Check if the given entity is alive
        [[nodiscard]] bool IsEntityAlive(Entity entity) const;
    };
}
